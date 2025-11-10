#pragma once
#include <fstream>
#include <type_traits>
#include "engine/utility/vector.h"
#include <string>
#include <sstream>

namespace serializer {
    class writer {
    public:
        void open(const std::string& filename);
        void close() { _file.close(); }

        template <typename T>
        void write(T var) {
            static_assert(std::is_trivially_copyable_v<T>);

            const auto* buf = reinterpret_cast<char*>(&var);
            _buffer.write(buf, sizeof(T));
        }

        template <typename T>
        void write(T* var, unsigned int count) {
            static_assert(std::is_trivially_copyable_v<T>);

            write(count);
            _buffer.write(reinterpret_cast<char*>(var), sizeof(T) * count);
        }

        void write(const char* var);
        void write(char* var);
        void write(const std::string& var, bool counted = true);

        std::string get_buffer() const { return _buffer.str(); }

        void clear();
        void flush();
    private:
        std::stringstream _buffer;
        std::ofstream _file;
    };

    class reader {
    public:
        void open_and_store(const std::string& filename);

        template<typename T>
        void read(T& var){
            static_assert(std::is_trivially_copyable_v<T>);

            assert_range<T>();
            memcpy(&var, _buffer.c_str() + _position, sizeof(T));

            _position += sizeof(T);
        }

        template<typename T>
        void read(T* ptr, unsigned int count) {
            static_assert(std::is_trivially_copyable_v<T>);
            assert_range<T>(count);

            memcpy(ptr, _buffer.c_str() + _position, count * sizeof(T));
            _position += count * sizeof(T);
        }

        template<typename T>
        void read(T* ptr) {
            static_assert(std::is_trivially_copyable_v<T>);

            unsigned int count;
            read(count);
            read(ptr, count);
        }

        void read(std::string& var);
        void read(std::string& var, unsigned int size);

        template<typename T>
        bool is_in_range() const{
            return is_in_range(sizeof(T));
        }

        std::string get_buffer() const { return _buffer; }
    private:
        template <typename T>
        void assert_range() const{
            assert(is_in_range(sizeof(T)));
        }

        template <typename T>
        void assert_range(unsigned int count) const{
            assert(is_in_range(sizeof(T) * count));
        }

        bool is_in_range(unsigned int size) const;

        std::ifstream _file;
        std::string _buffer;
        unsigned int _position = 0;
    };
}