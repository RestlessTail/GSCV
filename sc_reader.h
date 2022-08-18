#ifndef SC_READER
#define SC_READER

#include <filesystem>
#include <unordered_map>
#include <vector>
#include <string>
#include "utils.h"

class opengl_view_widget;

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#define SET_BINARY_MODE(file)
#endif

struct expr_index{
	size_t offset;
	size_t size;
};

struct gscv_archive_head{
	//read from head
	size_t cell_name_size;
	size_t meta_size;
	size_t position_size;
	size_t index_size;
	int n_cell;
	int n_gene;
	//calculate when reading archive
	size_t expr_dat_begin;
};

class sc_reader{
public:
	sc_reader();
	~sc_reader();
    int open(std::filesystem::path filename);
	float* get_cell_position();
	float* get_active_expr_data();
	int set_active_expr_data(std::string& gene_name);
	//test if a gene name exists in cell_expr_dict.
	//if the gene exists, return 0.
	//if the gene doesn't exist, return -1.
	int test_gene_name(std::string& gene_name);
	//test if a meta name exists in meta_dict.
	//if the name exists, return its column index.
	//if the name doesn't exist, return -1.
	int test_meta_name(std::string& meta_name);
	std::string& get_meta_val(int cell_index, int meta_index);
	std::vector<std::string>& get_meta_name();
	int get_n_cell();
	int get_n_gene();
	std::string get_archive_name();
	bool is_active;
private:
	std::vector<float> active_expr_data;
	std::vector<std::string> cell_name;
	std::vector<std::string> cell_meta;
	std::vector<float> cell_position;
	std::unordered_map<std::string, int> cell_meta_dict;
	std::unordered_map<std::string, expr_index> cell_expr_dict;
	std::vector<std::string> cell_meta_name;
	gscv_archive_head archive_head;
private:
	std::filesystem::path archive_filename;
	gscv_byte_stream_file file_strm;
private:
	int read_head();
	int read_cell_name();
	int read_cell_meta();
	int read_cell_position();
	int read_cell_expr_dict();
private:
	void clear();
private:
	int inflate_stream(gscv_byte_stream* src, gscv_byte_stream* dest);
    bool check_magic(int magic, gscv_byte_stream *stream);
};

#endif // SC_H
