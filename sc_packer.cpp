#include <filesystem>
#include <unordered_map>
#include <zlib.h>
#include "global_variables.h"
#include "sc_packer.h"
#include "utils.h"

void sc_packer::set_src_dir(std::filesystem::path p) {
    src_dir = p;
    cell_index = src_dir / "index.tsv";
    cell_expr = src_dir / "expr.tsv";
    cell_meta = src_dir / "meta.tsv";
    cell_position = src_dir / "position.tsv";
}
void sc_packer::set_dest_dir(std::filesystem::path p) {
    dest_dir = p;
    dest_head = dest_dir.string() + ".head";
    dest_index = dest_dir.string() + ".index";
    dest_data = dest_dir.string() + ".data";
}

void sc_packer::run() {
	int ret = -1;
	if((ret = check())){
		emit console_log(tr("An error occurred when checking files.\nExit."));
		return;
	}
	if((ret = pack())) {
		emit console_log(tr("An error occurred when packing files.\nExit."));
		return;
	}
    emit console_log(tr("The files are successfully packed."));
}

int sc_packer::pack() {
	// adjust progress bar
    emit progress_bar_max(n_gene + 3);
    cur_progress_bar_val = 0;
    emit progress_bar_val(cur_progress_bar_val);
    emit console_log(tr("Packing ") + QString(src_dir.string().c_str()) + tr("..."));

    // prepare file IO stream
    gscv_byte_stream_file *out_head = new gscv_byte_stream_file(dest_head, "wb");
    gscv_byte_stream_file *out_data = new gscv_byte_stream_file(dest_data, "wb");
    gscv_byte_stream_file *out_index = new gscv_byte_stream_file(dest_index, "wb");

    // add cell index
    int ret = -1;
    emit console_log(tr("Packing index..."));
    if ((ret = add_cell_name(out_data, out_head))) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        delete out_head;
        out_head = nullptr;
        delete out_index;
        out_index = nullptr;
        delete out_data;
        out_data = nullptr;
        std::filesystem::remove(dest_head);
        std::filesystem::remove(dest_index);
        std::filesystem::remove(dest_data);
        return ret;
    }
    emit progress_bar_val(++cur_progress_bar_val);
    // add meta data
    emit console_log(tr("Packing meta data..."));
    if ((ret = add_meta(out_data, out_head))) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        delete out_head;
        out_head = nullptr;
        delete out_index;
        out_index = nullptr;
        delete out_data;
        out_data = nullptr;
        std::filesystem::remove(dest_head);
        std::filesystem::remove(dest_index);
        std::filesystem::remove(dest_data);
        return ret;
    }
    emit progress_bar_val(++cur_progress_bar_val);
    // add cell position
    emit console_log(tr("Packing position..."));
    if ((ret = add_pos(out_data, out_head))) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        delete out_head;
        out_head = nullptr;
        delete out_index;
        out_index = nullptr;
        delete out_data;
        out_data = nullptr;
        std::filesystem::remove(dest_head);
        std::filesystem::remove(dest_index);
        std::filesystem::remove(dest_data);
        return ret;
    }
    emit progress_bar_val(++cur_progress_bar_val);
    // write expression data
    emit console_log(tr("Packing expression data..."));
    if ((ret = add_expr(out_data, out_head, out_index))) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        delete out_head;
        out_head = nullptr;
        delete out_index;
        out_index = nullptr;
        delete out_data;
        out_data = nullptr;
        std::filesystem::remove(dest_head);
        std::filesystem::remove(dest_index);
        std::filesystem::remove(dest_data);
        return ret;
    }
    emit progress_bar_val(++cur_progress_bar_val);
    // clean up
    delete out_head;
    out_head = nullptr;
    delete out_index;
    out_index = nullptr;
    delete out_data;
    out_data = nullptr;

    // catenate head, index and data
    cat_files();
    // clean up
    emit console_log(tr("Cleaning up..."));
    std::filesystem::remove(dest_head);
    std::filesystem::remove(dest_index);
    std::filesystem::remove(dest_data);

    return 0;
}

size_t sc_packer::deflate_stream(gscv_byte_stream *src, gscv_byte_stream *dest,
                                 int level) {
    int status, flush;
    size_t out_bytes = 0;
    unsigned have;
    z_stream strm;
    unsigned char* in = new unsigned char[global_variables::zlib_chunk_size];
    unsigned char* out = new unsigned char[global_variables::zlib_chunk_size];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    status = deflateInit(&strm, level);
    if (status != Z_OK){
        delete[] in;
        delete[] out;
        return out_bytes;
    }
    /* compress until end of file */
    do {
        strm.avail_in = src->read_from_stream(sizeof(char) * global_variables::zlib_chunk_size, in);
        flush = src->eof() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = global_variables::zlib_chunk_size;
            strm.next_out = out;
            status = deflate(&strm, flush); /* no bad return value */
            // assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = global_variables::zlib_chunk_size - strm.avail_out;
            if (dest->write_to_stream(out, sizeof(char), have) != have) {
                (void)deflateEnd(&strm);
                delete[] in;
                delete[] out;
                return out_bytes;
            }
            out_bytes += have;
        } while (strm.avail_out == 0);
        // assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    // assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    delete[] in;
    delete[] out;
    return out_bytes;
}

void sc_packer::append_magic(int magic, gscv_byte_stream *stream) {
    stream->write_to_stream(&magic, sizeof(int), 1);
}

void sc_packer::c_str_2_float_array(char *src, std::vector<float> &dest) {
    char *sep = nullptr;
    while ((sep = strchr(src, '\t'))) {
        *sep = 0;
        dest.push_back(atof(src));
        src = sep + 1;
    }
    dest.push_back(atof(src));
}

int sc_packer::check_index() {
    if (!std::filesystem::exists(cell_index)) {
        emit console_log(tr("Failed to find cell index file."));
        return -1;
    }
    int _n_cell = 0;
    char *buf = read_all(cell_index);
    char *buf_del = buf;
    char *end_of_line = nullptr;
    while ((end_of_line = strchr(buf, '\n'))) {
        *end_of_line = 0;
        cell_name_order.push_back(buf);
        cell_name_set.emplace(buf);
        ++_n_cell;
        buf = end_of_line + 1;
    }
    cell_name_order.push_back(buf);
    cell_name_set.emplace(buf);
    ++_n_cell;

    n_cell = _n_cell;
    delete[] buf_del;
    return 0;
}

int sc_packer::check_pos() {
    if (!std::filesystem::exists(cell_position)) {
        emit console_log(tr("Failed to find cell position file."));
        return -1;
    }
    char *buf = read_all(cell_position);
    char *buf_del = buf;
    char *end_of_line = nullptr;
    char *sep = nullptr;
    int n_col = 0;
    int _n_cell = 0;
    while ((end_of_line = strchr(buf, '\n'))) {
        *end_of_line = 0;
        n_col = 0;
        ++_n_cell;
        sep = strchr(buf, '\t');
        *sep = 0;
        if(cell_name_set.find(buf) == cell_name_set.end()){
            emit console_log(tr("Cell names in cell index don't match cell names in cell position file."));
            delete[] buf_del;
            return -1;
        }
        buf = sep + 1;
        while ((sep = strchr(buf, '\t'))) {
            ++n_col;
            buf = sep + 1;
        }
        ++n_col;
        if (n_col != 2) {
            emit console_log(tr("Wrong column number in cell position file."));
            delete[] buf_del;
            return -1;
        }
        buf = end_of_line + 1;
    }
    n_col = 0;
    ++_n_cell;
    sep = strchr(buf, '\t');
    *sep = 0;
    if(cell_name_set.find(buf) == cell_name_set.end()){
        emit console_log(tr("Cell names in cell index don't match cell names in cell position file."));
        delete[] buf_del;
        return -1;
    }
    buf = sep + 1;
    while ((sep = strchr(buf, '\t'))) {
        ++n_col;
        buf = sep + 1;
    }
    ++n_col;
    if (n_col != 2) {
        emit console_log(tr("Wrong column number in cell position file."));
        delete[] buf_del;
        return -1;
    }

    if (_n_cell != n_cell) {
        emit console_log(tr("Wrong cell number in cell position file."));
        delete[] buf_del;
        return -1;
    }
    delete[] buf_del;
    return 0;
}

int sc_packer::check_meta() {
    if (!std::filesystem::exists(cell_meta)) {
        emit console_log(tr("Failed to find cell meta file."));
        return -1;
    }
    char *buf = read_all(cell_meta);
    char *buf_del = buf;
    char *end_of_line = nullptr;
    char *sep = nullptr;
    int _n_meta = 0;
    int _n_cell = 0;
    //skip header
    end_of_line = strchr(buf, '\n');
    buf = end_of_line + 1;
    //read meta data
    while ((end_of_line = strchr(buf, '\n'))) {
        *end_of_line = 0;
        ++_n_cell;
        sep = strchr(buf, '\t');
        *sep = 0;
        if(cell_name_set.find(buf) == cell_name_set.end()){
            emit console_log(tr("Cell names in cell index don't match cell names in meta data file."));
            delete[] buf_del;
            return -1;
        }
        buf = sep + 1;
        while ((sep = strchr(buf, '\t'))) {
            ++_n_meta;
            buf = sep + 1;
        }
        ++_n_meta;
        n_meta = _n_meta;
        buf = end_of_line + 1;
    }
    ++_n_cell;
    sep = strchr(buf, '\t');
    *sep = 0;
    if(cell_name_set.find(buf) == cell_name_set.end()){
        delete[] buf_del;
        return -1;
    }
    buf = sep + 1;
    while ((sep = strchr(buf, '\t'))) {
        ++_n_meta;
        buf = sep + 1;
    }
    ++_n_meta;
    n_meta = _n_meta;

    if (_n_cell != n_cell) {
        emit console_log(tr("Wrong cell number in cell position file."));
        delete[] buf_del;
        return -1;
    }
    delete[] buf_del;
    return 0;
}

int sc_packer::check_expr() {
    if (!std::filesystem::exists(cell_expr)) {
        emit console_log(tr("Failed to find expression data file."));
        return -1;
    }
    file_reader reader(cell_expr, "r");
    char *buf = nullptr;
    char *buf_del = nullptr;
    char *sep = nullptr;
    int _n_gene = 0;
    int _n_cell = 0;
    while ((buf = reader.get_next_line())) {
        buf_del = buf;
        ++_n_gene;
        _n_cell = 0;
        while ((sep = strchr(buf, '\t'))) {
            ++_n_cell;
            buf = sep + 1;
        }
        if (n_cell != _n_cell) {
            emit console_log(tr("Wrong column number in expression data file."));
            delete[] buf_del;
            return -1;
        }
        delete [] buf_del;
    }
    n_gene = _n_gene;
    return 0;
}

int sc_packer::add_cell_name(gscv_byte_stream_file *out_data, gscv_byte_stream_file *out_head) {
    size_t deflate_size = 0;
    //size_t offset = out_data->offset();
    append_magic(global_variables::GSCV_ARCHIVE_MAGIC, out_data); // write magic number
    gscv_byte_stream_file *in = new gscv_byte_stream_file(cell_index, "r");
    if (in == nullptr) {
        emit console_log(tr("Failed to open cell index file."));
        delete in;
        in = nullptr;
        return -2;
    }
    deflate_size = deflate_stream(in, out_data, 5); // deflate & write
    delete in;
    in = nullptr;

    out_head->write_to_stream(&deflate_size, sizeof(size_t), 1);
    return 0;
}

int sc_packer::add_meta(gscv_byte_stream_file *out_data, gscv_byte_stream_file *out_head) {
    // prepare tmp variables
    std::unordered_map<std::string, std::string> tmp_map;
    gscv_byte_stream_memory *sorted_meta = new gscv_byte_stream_memory;
    // read raw data
    char *buf = read_all(cell_meta);
    char *buf_del = buf;
    char *end_of_line = nullptr;
    char *sep = nullptr;
    char *_cell_name = nullptr;
    // read headers
    end_of_line = strchr(buf, '\n');
    *end_of_line = 0;
    sorted_meta->write_to_stream(buf, sizeof(char), strlen(buf));
    sorted_meta->write_to_stream("\n", sizeof(char), 1);
    buf = end_of_line + 1;
    // read meta data
    while ((end_of_line = strchr(buf, '\n'))) {
        *end_of_line = 0;
        sep = strchr(buf, '\t');
        *sep = 0;
        _cell_name = buf;
        buf = sep + 1;
        tmp_map.emplace(_cell_name, buf);
		buf = end_of_line + 1;
    }
    sep = strchr(buf, '\t');
    *sep = 0;
    _cell_name = buf;
    buf = sep + 1;
    tmp_map.emplace(_cell_name, buf);

    // sort meta data
    for (auto &i : cell_name_order) {
        auto itr = tmp_map.find(i);
        if (itr == tmp_map.end()) {
            emit console_log(tr("Cell names in cell index don't match cell names in meta data file."));
            delete[] buf_del;
            delete sorted_meta;
            return -1;
        }
        std::string &meta = itr->second;
        sorted_meta->write_to_stream(meta.c_str(), sizeof(char), meta.size());
        sorted_meta->write_to_stream("\n", sizeof(char), 1);
    }

    // deflate & write
    size_t deflate_size = 0;
    append_magic(global_variables::GSCV_ARCHIVE_MAGIC, out_data);              // write magic number
    deflate_size = deflate_stream(sorted_meta, out_data, 5); // deflate & write
    out_head->write_to_stream(&deflate_size, sizeof(size_t), 1); // write index

    // clean up
    delete[] buf_del;
    delete sorted_meta;
    return 0;
}

int sc_packer::add_pos(gscv_byte_stream_file *out_data, gscv_byte_stream_file *out_head) {
    // prepare tmp variables
    std::unordered_map<std::string, std::pair<float, float>> tmp_map;
    gscv_byte_stream_memory *sorted_pos = new gscv_byte_stream_memory;
    // read pos data
    char *buf = read_all(cell_position);
    char *buf_del = buf;
    char *end_of_line = nullptr;
    char *sep = nullptr;
    char *cell_name = nullptr;
    float x_pos = 0.0f;
    float y_pos = 0.0f;
    while ((end_of_line = strchr(buf, '\n'))) {
        *end_of_line = 0;
        sep = strchr(buf, '\t');
        *sep = 0;
        cell_name = buf;
        buf = sep + 1;
        sep = strchr(buf, '\t');
        *sep = 0;
        x_pos = atof(buf);
        buf = sep + 1;
        y_pos = atof(buf);
        std::pair<float, float> pos;
        pos.first = x_pos;
        pos.second = y_pos;
        tmp_map.emplace(cell_name, pos);
		buf = end_of_line + 1;
    }
    sep = strchr(buf, '\t');
    *sep = 0;
    cell_name = buf;
    buf = sep + 1;
    sep = strchr(buf, '\t');
    *sep = 0;
    x_pos = atof(buf);
    buf = sep + 1;
    y_pos = atof(buf);
    std::pair<float, float> pos;
    pos.first = x_pos;
    pos.second = y_pos;
    tmp_map.emplace(cell_name, pos);

    // sort pos data
    for (auto &i : cell_name_order) {
        auto itr = tmp_map.find(i);
        if (itr == tmp_map.end()) {
            emit console_log(tr("Cell names in cell index don't match cell names in cell position file."));
            delete[] buf_del;
            delete sorted_pos;
            return -1;
        }
        std::pair<float, float> p = itr->second;
        sorted_pos->write_to_stream(&(p.first), sizeof(float), 1);
        sorted_pos->write_to_stream(&(p.second), sizeof(float), 1);
    }

    // deflate & write
    size_t deflate_size = 0;
    append_magic(global_variables::GSCV_ARCHIVE_MAGIC, out_data);             // write magic number
    deflate_size = deflate_stream(sorted_pos, out_data, 5); // deflate & write
    out_head->write_to_stream(&deflate_size, sizeof(size_t), 1); // write index

    // clean up
    delete[] buf_del;
    delete sorted_pos;
    return 0;
}

int sc_packer::add_expr(gscv_byte_stream_file *out_data, gscv_byte_stream_file* out_head, gscv_byte_stream_file *out_index) {
	size_t offset = 0;
	size_t deflate_size = 0;
    file_reader reader(cell_expr, "r");
    char *buf = nullptr;
    char *buf_del = nullptr;
    char *sep = nullptr;
    char *gene_name = nullptr;
    append_magic(global_variables::GSCV_ARCHIVE_MAGIC, out_index);
    while ((buf = reader.get_next_line())) {
        buf_del = buf;
        sep = strchr(buf, '\t'); // first column: gene name
        *sep = 0;
        gene_name = buf;
		emit console_log(tr("    Packing ") + QString(gene_name));
        buf = sep + 1;
        std::vector<float> expr_data; // the rest columns: expression data
        c_str_2_float_array(buf, expr_data);
        // build index
        offset = out_data->offset();
        out_index->write_to_stream(gene_name, sizeof(char), strlen(gene_name));
        out_index->write_to_stream("\t", sizeof(char), 1);
        out_index->write_to_stream(&offset, sizeof(size_t), 1);
        // write data
        append_magic(global_variables::GSCV_ARCHIVE_MAGIC, out_data);
        gscv_byte_stream_memory byte_stream;
        byte_stream.write_to_stream(expr_data.data(), sizeof(float), expr_data.size());
        deflate_size = deflate_stream(&byte_stream, out_data, 5);
        // build index
        out_index->write_to_stream(&deflate_size, sizeof(size_t), 1);
        out_index->write_to_stream("\n", sizeof(char), 1);
        // next line
        delete [] buf_del;
        emit progress_bar_val(++cur_progress_bar_val);
    }

    // write head
    deflate_size = out_index->size() - sizeof(int);
    out_head->write_to_stream(&deflate_size, sizeof(size_t), 1);
    out_head->write_to_stream(&n_cell, sizeof(int), 1);
    out_head->write_to_stream(&n_gene, sizeof(int), 1);

	return 0;
}

int sc_packer::cat_files(){
    emit console_log(tr("Catenating files..."));
    // prepare file IO stream
    gscv_byte_stream_file* in_head = new gscv_byte_stream_file(dest_head, "rb");
    gscv_byte_stream_file* in_index = new gscv_byte_stream_file(dest_index, "rb");
    gscv_byte_stream_file* in_data = new gscv_byte_stream_file(dest_data, "rb");
    gscv_byte_stream_file *out = new gscv_byte_stream_file(dest_dir, "wb");
    if (out == nullptr) {
        emit console_log(tr("Cannot open output file."));
        delete out;
        return -2;
    }

    in_head->transfer(out, sizeof(char), in_head->size());
    in_index->transfer(out, sizeof(char), in_index->size());
    in_data->transfer(out, sizeof(char), in_data->size());
    
    delete in_head;
    in_head = nullptr;
    delete in_index;
    in_index = nullptr;
    delete in_data;
    in_data = nullptr;
    delete out;
    out = nullptr;
	return 0;
}

int sc_packer::check(){
    emit progress_bar_max(4);
    emit progress_bar_val(0);
    int ret = -1;
    emit console_log(tr("Checking index..."));
    if ((ret = check_index())) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        return ret;
    } else {
        emit console_log(tr("    Find ") + QString::number(n_cell) + tr(" cells."));
        emit console_log(tr("    Pass."));
    }
    emit progress_bar_val(1);
    emit console_log(tr("Checking position..."));
    if ((ret = check_pos())) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        return ret;
    } else {
        emit console_log(tr("    Pass."));
    }
    emit progress_bar_val(2);
    emit console_log(tr("Checking meta data..."));
    if ((ret = check_meta())) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        return ret;
    } else {
        emit console_log(tr("    Find ") + QString::number(n_meta) +
                         tr(" meta data."));
        emit console_log(tr("    Pass."));
    }
    emit progress_bar_val(3);
    emit console_log(tr("Checking expression data..."));
    if ((ret = check_expr())) {
        emit console_log(tr("    Failed."));
        emit console_log(tr("Stop."));
        return ret;
    } else {
        emit console_log(tr("    Find ") + QString::number(n_gene) + tr(" genes."));
        emit console_log(tr("    Pass."));
    }
    emit progress_bar_val(4);
	return 0;
}
