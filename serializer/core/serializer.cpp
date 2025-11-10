#include "serializer/core/serializer.h"
#include <sstream>

namespace serializer {

    void writer::open(const std::string& filename) {
        _file.open(filename, std::ios::out | std::ios::binary);
    }
    void writer::write(const char* var) {
        write(std::string(var));
    }

    void writer::write(char* var) {
        write(std::string(var));
    }

    void writer::write(const std::string& var, bool counted) {
        if ( counted )
            write(static_cast<unsigned int>(var.size()));
        _buffer << var;
    }

    void writer::clear() {
        _buffer.str("");
    }

    void writer::flush() {
        assert(_file.is_open());
        std::string str = _buffer.str();
        _file << _buffer.str();
        _buffer.clear();
    }

    void reader::open_and_store(const std::string& filename) {
        _file.open(filename, std::ios::in | std::ios::binary);
        std::stringstream ss;
        ss << _file.rdbuf();
        _buffer = ss.str();

        _file.close();
    }

    bool reader::is_in_range(unsigned int size) const{
        return _position + size <= _buffer.size();
    }

    void reader::read(std::string& var, unsigned int size) {
        var = _buffer.substr(_position, size);
        _position += size;
    }

    void reader::read(std::string& var) {
        unsigned int size;
        read(size);
        read(var, size);
    }


}