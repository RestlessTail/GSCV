#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <string.h>
#include <zlib.h>
#include "global_variables.h"
#include "sc_reader.h"
#include "utils.h"

sc_reader::sc_reader(){
    clear();
}

sc_reader::~sc_reader(){
}

int sc_reader::open(std::filesystem::path filename) {
    clear();
    archive_filename = filename;
    int ret = -1;
    if((ret = file_strm.open(filename, "rb"))){
        return ret;
    }
    if((ret = read_head())){
        return ret;
    }
    if((ret = read_cell_expr_dict())){
        return ret;
    }
    if((ret = read_cell_name())){
        return ret;
    }
    if((ret = read_cell_meta())){
        return ret;
    }
    if((ret = read_cell_position())){
        return ret;
    }
    file_strm.close();
    is_active = true;
    return 0;
}

int sc_reader::read_head() {
    size_t head[4] = { 0 };
    size_t have = 0;
    have = file_strm.read_from_stream(4 * sizeof(size_t), head);
    if(have != 4 * sizeof(size_t)){
        return -1;
    }
    archive_head.cell_name_size = head[0];
    archive_head.meta_size = head[1];
    archive_head.position_size = head[2];
    archive_head.index_size = head[3];
    archive_head.expr_dat_begin =
        4 * sizeof(size_t) + 2 * sizeof(int) // head
        /* + 4 * sizeof(int)  // magic index
        + archive_head.cell_name_size
        + archive_head.meta_size
        + archive_head.position_size*/
        + archive_head.index_size + sizeof(int);
    int n_cell;
    have = file_strm.read_from_stream(1 * sizeof(int), &n_cell);
    if(have != 1 * sizeof(int)){
        return -1;
    }
    archive_head.n_cell = n_cell;
    int n_gene;
    have = file_strm.read_from_stream(1 * sizeof(int), &n_gene);
    if(have != 1 * sizeof(int)){
        return -1;
    }
    archive_head.n_gene = n_gene;
    return 0;
}

int sc_reader::read_cell_name() {
    int o = file_strm.offset();
    //read deflated data from archive
    if(!check_magic(global_variables::GSCV_ARCHIVE_MAGIC, &file_strm)){
        return -1;
    }
    gscv_byte_stream_memory raw;
    file_strm.transfer(&raw, sizeof(char), archive_head.cell_name_size);
    //inflate data
    gscv_byte_stream_memory inflated;
    inflate_stream(&raw, &inflated);
    raw.clear();
    //copy to char array
    char* buf =  new char[inflated.size() + 1];
    memset(buf, 0, inflated.size() + 1);
    inflated.read_from_stream(sizeof(char) * inflated.size(), buf);
    strip_right(buf);
    inflated.clear();
    //parse buf
    char* buf_del = buf;
    char* end_of_line = nullptr;
    while((end_of_line = strchr(buf, '\n'))){
        *end_of_line = 0;
        cell_name.push_back(buf);
        buf = end_of_line + 1;
    }
    cell_name.push_back(buf);
    if(archive_head.n_cell != cell_name.size()){
        delete [] buf_del;
        return -1;
    }
    //clean up
    delete [] buf_del;
    return 0;
}
int sc_reader::read_cell_meta() {
    //read deflated data from archive
    if(!check_magic(global_variables::GSCV_ARCHIVE_MAGIC, &file_strm)){
        return -1;
    }
    gscv_byte_stream_memory raw;
    file_strm.transfer(&raw, sizeof(char), archive_head.meta_size);
    //inflate data
    gscv_byte_stream_memory inflated;
    inflate_stream(&raw, &inflated);
    raw.clear();
    //copy to char array
    char* buf =  new char[inflated.size() + 1];
    memset(buf, 0, inflated.size() + 1);
    inflated.read_from_stream(sizeof(char) * inflated.size(), buf);
    strip_right(buf);
    inflated.clear();
    //parse buf
    char* buf_del = buf;
    char* end_of_line = nullptr;
    char* sep = nullptr;
    //parse header
    int cur_meta_col = 0;
    end_of_line = strchr(buf, '\n');
    *end_of_line = 0;
    while((sep = strchr(buf, '\t'))){
        *sep = 0;
        cell_meta_dict.emplace(std::string(buf), cur_meta_col++);
        cell_meta_name.push_back(std::string(buf));
        buf = sep + 1;
    }
    cell_meta_dict.emplace(std::string(buf), cur_meta_col);
    buf = end_of_line + 1;
    //parse meta data
    while((end_of_line = strchr(buf, '\n'))){
        *end_of_line = 0;
        while((sep = strchr(buf, '\t'))){
            *sep = 0;
            cell_meta.push_back(std::string(buf));
            buf = sep + 1;
        }
        cell_meta.push_back(std::string(buf));
        buf = end_of_line + 1;
    }
    while((sep = strchr(buf, '\t'))){
        *sep = 0;
        cell_meta.push_back(std::string(buf));
        buf = sep + 1;
    }
    cell_meta.push_back(std::string(buf));
    //clean up
    delete [] buf_del;
    return 0;
}

int sc_reader::read_cell_position() {
    //read deflated data from archive
    if(!check_magic(global_variables::GSCV_ARCHIVE_MAGIC, &file_strm)){
        return -1;
    }
    gscv_byte_stream_memory raw;
    file_strm.transfer(&raw, sizeof(char), archive_head.position_size);
    //inflate data
    gscv_byte_stream_memory inflated;
    inflate_stream(&raw, &inflated);
    raw.clear();
    //copy to float array
    int n_cell = get_n_cell();
    if(inflated.size() * sizeof(char) / sizeof(float) != static_cast<size_t>(n_cell) * 2){
        return -1;
    }
    float* pos = new float[n_cell * 2];
    inflated.read_from_stream(sizeof(char) * inflated.size(), pos);
    inflated.clear();
    //read float array
    cell_position = std::vector<float>(pos, pos + n_cell * 2);
    //clean up
    delete [] pos;
    return 0;
}

int sc_reader::read_cell_expr_dict() {
    //read deflated data from archive
    if(!check_magic(global_variables::GSCV_ARCHIVE_MAGIC, &file_strm)){
        return -1;
    }
    gscv_byte_stream_memory raw;
    file_strm.transfer(&raw, sizeof(char), archive_head.index_size);
    //copy to char array
    char* buf =  new char[raw.size()];
    char* buf_del = buf;
    char* sep = nullptr;
    char* gene_name = nullptr;
    size_t _expr_index[2];
    raw.read_from_stream(sizeof(char) * raw.size(), buf);
    //parse char array
    int n_gene = get_n_gene();
    for(int i = 0; i < n_gene; ++i){
        sep = strchr(buf, '\t');
        *sep = 0;
        gene_name = buf;
        buf = sep + 1;
        memcpy(_expr_index, buf, 2 * sizeof(size_t));
        buf = buf + 2 * sizeof(size_t) / sizeof(char);
        if(*buf != '\n'){
            delete[] buf_del;
            return -1;
        }
        buf = buf + 1;
        expr_index e;
        e.offset = _expr_index[0];
        e.size = _expr_index[1];
        cell_expr_dict.emplace(std::string(gene_name), e);
    }
    if (buf - buf_del != archive_head.index_size) {
        delete[] buf_del;
        return -1;
    }
    //clean up
    delete [] buf_del;
    return 0;
}

int sc_reader::inflate_stream(gscv_byte_stream *src, gscv_byte_stream *dest) {
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char* in = new unsigned char[global_variables::zlib_chunk_size];
    unsigned char* out = new unsigned char[global_variables::zlib_chunk_size];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK){
        delete[] in;
        delete[] out;
        return ret;
    }

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = src->read_from_stream(sizeof(char) * global_variables::zlib_chunk_size, in);
        if (strm.avail_in == 0) {
            break;
        }
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = global_variables::zlib_chunk_size;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            // assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR; /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                delete[] in;
                delete[] out;
                return ret;
            }
            have = global_variables::zlib_chunk_size - strm.avail_out;
            if (dest->write_to_stream(out, sizeof(char), have) != have) {
                (void)inflateEnd(&strm);
                delete[] in;
                delete[] out;
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    delete[] in;
    delete[] out;
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

bool sc_reader::check_magic(int magic, gscv_byte_stream *stream) {
    int magic_read;
    stream->read_from_stream(sizeof(int) * 1, &magic_read);
    return (magic_read == magic);
}

void sc_reader::clear(){
    is_active = false;
    active_expr_data.clear();
    cell_name.clear();
    cell_meta.clear();
    cell_position.clear();
    cell_meta_dict.clear();
    cell_expr_dict.clear();
    cell_meta_name.clear();
    archive_head.cell_name_size = 0;
    archive_head.meta_size = 0;
    archive_head.position_size = 0;
    archive_head.index_size = 0;
    archive_head.expr_dat_begin = 0;
    archive_head.n_cell = 0;
    archive_head.n_gene = 0;
    archive_filename.clear();
    file_strm.close();
}

float* sc_reader::get_cell_position(){
    return cell_position.data();
}

float* sc_reader::get_active_expr_data(){
    return active_expr_data.data();
}

int sc_reader::set_active_expr_data(std::string& gene_name){
    //resize vector
    int n_cell = get_n_cell();
    if(active_expr_data.size() != n_cell){
        active_expr_data.resize(n_cell);
    }
    //find gene
    auto itr = cell_expr_dict.find(gene_name);
    if(itr == cell_expr_dict.end()){
        return -1;
    }
    //read data
    file_strm.open(archive_filename, "rb");
    file_strm.move_to(archive_head.expr_dat_begin + itr->second.offset);
    check_magic(global_variables::GSCV_ARCHIVE_MAGIC, &file_strm);
    gscv_byte_stream_memory raw;
    file_strm.transfer(&raw, sizeof(char), itr->second.size);
    file_strm.close();
    //inflate
    gscv_byte_stream_memory inflated;
    inflate_stream(&raw, &inflated);
    raw.clear();
    //copy to float array
    size_t array_size = n_cell;
    if(inflated.size() * sizeof(char) / sizeof(float) != array_size){
        return -1;
    }
    float* tmp = new float[array_size];
    inflated.read_from_stream(inflated.size() * sizeof(char), tmp);
    //copy to float vector
    active_expr_data = std::vector<float>(tmp, tmp + array_size);
    //clean up
    delete [] tmp;
    return 0;
}

std::string& sc_reader::get_meta_val(int cell_index, int meta_index){
    return cell_meta[cell_index * cell_meta_dict.size() + meta_index];
}

int sc_reader::test_gene_name(std::string &gene_name){
    auto itr = cell_expr_dict.find(gene_name);
    if (itr == cell_expr_dict.end()){
        return -1;
    }
    else{
        return 0;
    }
}

int sc_reader::test_meta_name(std::string &meta_name){
    auto itr = cell_meta_dict.find(meta_name);
    if(itr == cell_meta_dict.end()){
        return -1;
    }
    else{
        return itr->second;
    }
}
std::vector<std::string>& sc_reader::get_meta_name(){
    return cell_meta_name;
}
int sc_reader::get_n_cell(){
    return archive_head.n_cell;
}
int sc_reader::get_n_gene(){
    return archive_head.n_gene;
}
std::string sc_reader::get_archive_name(){
    std::string archive_path = archive_filename.string();
    auto slash = archive_path.rfind("/");
    auto extension = archive_path.rfind(".");
    if(slash == std::string::npos || extension == std::string::npos || extension <= slash){
        return std::string();
    }
    return archive_path.substr(slash + 1, extension - slash - 1);;
}
