#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include <ctime>

namespace utl::uid {
    inline std::string get_uid() {
        static long long int current_id = 0;

        std::ostringstream ss;
        const auto t = std::time(nullptr);
        const auto tm = *std::localtime(&t);
        ss << std::put_time(&tm, "%Y-%m-%d-%H:%M:%S");

        return ss.str() + '_' + std::to_string(current_id++);
    }

    inline std::string get_prefix_uid(const std::string& prefix = "") {
        return prefix + get_uid();
    }
}