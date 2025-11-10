#pragma once
#include <filesystem>

namespace serializer::project::active {

    std::filesystem::path get_project_path();
    std::string get_active_scene();

    void save_detail();
    void save_as(const std::filesystem::path& path, const std::string& name);
    void save();
    void load(const std::filesystem::path& path);

}