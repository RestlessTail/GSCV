#include <filesystem>
#include <chrono>
#include <queue>
#include <stdio.h>
#include <string.h>
#include "utils.h"

time_profiler::time_profiler() {
    start = std::chrono::system_clock::now();
}

long long time_profiler::get_micro_sec() {
    std::chrono::system_clock::time_point end;
    end = std::chrono::system_clock::now();
    auto ret = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return ret.count();
}

size_t gscv_byte_stream::transfer(gscv_byte_stream* dest, size_t ele_size, size_t ele_count) {
    size_t have1 = 0;
    size_t have2 = 0;
    char* buf = new char[ele_size * ele_count];
    have1 = read_from_stream(ele_size * ele_count, buf);
    have2 = dest->write_to_stream(buf, sizeof(char), have1);
    return have1 == have2 ? have1 : -1;
}


size_t gscv_byte_stream_memory::read_from_stream(size_t size, void *dest) {
    char* _dest = (char*)dest;
    if(stream_data.size() < size) {
        size = stream_data.size();
    }
    for(size_t i = 0; i < size; ++i) {
        *(_dest + i) = stream_data.front();
        stream_data.pop();
    }
    return size;
}

size_t gscv_byte_stream_memory::write_to_stream(const void *data, size_t ele_size, size_t ele_count) {
    char* _data = (char*)data;
    size_t size = ele_count * ele_size;
    for(size_t i = 0; i < size; ++i) {
        stream_data.push(*(_data + i));
    }
    return ele_count;
}

bool gscv_byte_stream_memory::eof() {
    return (stream_data.size() == 0);
}

size_t gscv_byte_stream_memory::size() {
    return stream_data.size();
}

void gscv_byte_stream_memory::clear() {
    std::queue<char> tmp;
    std::swap(tmp, stream_data);
}

gscv_byte_stream_file::gscv_byte_stream_file() {
    fp = nullptr;
}

gscv_byte_stream_file::gscv_byte_stream_file(std::filesystem::path filename, const char* mode) {
    fp = fopen(filename.string().c_str(), mode);
}

gscv_byte_stream_file::~gscv_byte_stream_file() {
    if(fp) {
        fclose(fp);
        fp = nullptr;
    }
}

int gscv_byte_stream_file::open(std::filesystem::path filename, const char* mode) {
    if(fp) {
        fclose(fp);
        fp = nullptr;
    }
    fp = fopen(filename.string().c_str(), mode);
    if (fp) {
        return 0;
    }
    else {
        return -1;
    }
}

void gscv_byte_stream_file::close(){
    if(fp){
        fclose(fp);
        fp = nullptr;
    }
}

void gscv_byte_stream_file::move_to(size_t offset){
    fseek(fp, offset, SEEK_SET);
}

size_t gscv_byte_stream_file::read_from_stream(size_t size, void* dest) {
    return std::fread(dest, sizeof(char), size, fp);
}

size_t gscv_byte_stream_file::write_to_stream(const void *data, size_t ele_size, size_t ele_count) {
    return std::fwrite(data, ele_size, ele_count, fp);
}

bool gscv_byte_stream_file::eof() {
    return feof(fp);
}

size_t gscv_byte_stream_file::size() {
    return get_file_size(fp);
}

size_t gscv_byte_stream_file::offset() {
    return static_cast<size_t>(ftell(fp));
}

file_reader::file_reader(std::filesystem::path filename, const char* mode){
    fp = fopen(filename.string().c_str(), mode);
    fread_buf = new char[chunk_size()];
}

file_reader::~file_reader(){
    fclose(fp);
    delete [] fread_buf;
}

char* file_reader::get_next_line(){
    while(1){
        if(!this_line.empty()){
            size_t pos = this_line.find('\n');
            if(pos != std::string::npos){
                char* ret = new char[pos + 1];
                strncpy(ret, this_line.c_str(), pos);
                ret[pos] = 0;
                this_line = this_line.substr(pos + 1);
                return ret;
            }
        }
        if(feof(fp)){
            return nullptr;
        }
        have = fread(fread_buf, sizeof(char), chunk_size(), fp);
        this_line.insert(this_line.size(), fread_buf, have);
    }
}

char* read_all(const char* filename, size_t* size, bool is_strip_right) {
    FILE* fp = fopen(filename, "r");
    if(!fp) {
        return nullptr;
    }
    fseek(fp, 0, SEEK_END);
    size_t file_size = ftell(fp);
    char* ret = new char[file_size + 1];
    memset(ret, 0, file_size + 1);
    fseek(fp, 0, SEEK_SET);
    size_t have = fread(ret, sizeof(char), file_size, fp);
    fclose(fp);
    fp = nullptr;
    if (size) {
        *size = have;
    }
    if(is_strip_right) {
        strip_right(ret);
    }
    return ret;
}

char* read_all(std::filesystem::path filename, size_t* size, bool is_strip_right) {
    return read_all(filename.string().c_str(), size, is_strip_right);
}

void strip_right(char* s) {
    size_t have = strlen(s);
    char* rbegin = s + have - 1;
    int offset = 0;
    while(isspace(*(rbegin - offset))) {
        *(rbegin - offset) = 0;
    }
}

size_t get_file_size(FILE* fp) {
    size_t pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    size_t ret = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return ret;
}

size_t get_file_size(std::filesystem::path file) {
    FILE* fp = fopen(file.string().c_str(), "r");
    size_t ret = get_file_size(fp);
    fclose(fp);
    fp = nullptr;
    return ret;
}
