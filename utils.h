#ifndef UTILS
#define UTILS

#include <filesystem>
#include <queue>
#include <stdio.h>

class time_profiler {
public:
    time_profiler();
    long long get_micro_sec();
private:
    std::chrono::system_clock::time_point start;
};

class gscv_byte_stream {
public:
    virtual size_t transfer(gscv_byte_stream* dest, size_t ele_size, size_t ele_count);
	virtual ~gscv_byte_stream(){}
    virtual size_t read_from_stream(size_t size, void* dest) = 0;
    virtual size_t write_to_stream(const void* data, size_t ele_size, size_t ele_count) = 0;
	virtual bool eof() = 0;
	virtual size_t size() = 0;
};

class gscv_byte_stream_memory : public gscv_byte_stream {
public:
    size_t read_from_stream(size_t size, void* dest) override;
    size_t write_to_stream(const void* data, size_t ele_size, size_t ele_count) override;
	bool eof() override;
	size_t size() override;
    void clear();
private:
    std::queue<char> stream_data;
};

class gscv_byte_stream_file : public gscv_byte_stream {
public:
    gscv_byte_stream_file(std::filesystem::path filename, const char* mode);
    gscv_byte_stream_file();
    int open(std::filesystem::path filename, const char* mode);
    void close();
    void move_to(size_t offset);
    ~gscv_byte_stream_file();
    size_t read_from_stream(size_t size, void* dest) override;
    size_t write_to_stream(const void* data, size_t ele_size, size_t ele_count) override;
	bool eof() override;
	size_t size() override;
	size_t offset();
private:
    FILE* fp;
};

class file_reader{
public:
    file_reader(std::filesystem::path filename, const char* mode);
    ~file_reader();
    char* get_next_line();
private:
    FILE* fp;
    size_t have;
    std::string this_line;
    char* fread_buf;
    constexpr size_t chunk_size(){ return 2048; }
};

char* read_all(const char* filename, size_t* size = nullptr, bool is_strip_right = true);

char* read_all(std::filesystem::path filename, size_t* size = nullptr, bool is_strip_right = true);

void strip_right(char* s);

size_t get_file_size(FILE* fp);

size_t get_file_size(std::filesystem::path file);

float safe_atof(const char* s);

#endif