#pragma once
#include <filesystem>
#include <fstream>

namespace serializer::file::temp {

    void delete_temporary_file(const std::string& uid);
    void delete_temporary_files();

    std::filesystem::path get_tmp_path_from_uid(const std::string& uid);
}