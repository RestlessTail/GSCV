#ifndef SC_PACKER
#define SC_PACKER

#include <QThread>
#include <filesystem>
#include <unordered_set>

class gscv_byte_stream;
class gscv_byte_stream_file;

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#define SET_BINARY_MODE(file)
#endif

//Usage:
//    obj->set_src_dir("src");
//    obj->set_dest_dir("dest");
//    obj->start();
class sc_packer : public QThread {
    Q_OBJECT
public:
    void run() override;
    void set_src_dir(std::filesystem::path p);
    void set_dest_dir(std::filesystem::path p);
signals:
    void console_log(QString s);
    void progress_bar_max(int val);
    void progress_bar_val(int val);
private:
    std::filesystem::path src_dir;
    std::filesystem::path dest_dir;
private:
    std::filesystem::path cell_index;
    std::filesystem::path cell_expr;
    std::filesystem::path cell_meta;
    std::filesystem::path cell_position;
    std::filesystem::path dest_head;
    std::filesystem::path dest_index;
    std::filesystem::path dest_data;
private:
    std::vector<std::string> cell_name_order;
    std::unordered_set<std::string> cell_name_set;
private:
    int n_cell;
    int n_meta;
    int n_gene;
private:
    int cur_progress_bar_val;
private:
    int check_index();
    int check_meta();
    int check_pos();
    int check_expr();
    int add_cell_name(gscv_byte_stream_file* out_data, gscv_byte_stream_file* out_head);
    int add_meta(gscv_byte_stream_file* out_data, gscv_byte_stream_file* out_head);
    int add_pos(gscv_byte_stream_file* out_data, gscv_byte_stream_file* out_head);
    int add_expr(gscv_byte_stream_file* out_data, gscv_byte_stream_file* out_head, gscv_byte_stream_file* out_index);
    int cat_files();
private:
    int pack();
    int check();
    size_t deflate_stream(gscv_byte_stream *src, gscv_byte_stream *dest, int level);
    void append_magic(int magic, gscv_byte_stream *stream);
    void c_str_2_float_array(char *src, std::vector<float> &dest);
};

#endif
