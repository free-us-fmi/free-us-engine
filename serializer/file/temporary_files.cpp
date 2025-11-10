#include "temporary_files.h"

#include <cassert>

#include "engine/utility/uid.h"
#include "project/active_project.h"
#include <fstream>
#include <iostream>

namespace serializer::file::temp {
    constexpr const char* extension = "sort";

    namespace {
        std::filesystem::path get_project_temporary_directory() {
            std::filesystem::path project_path = project::active::get_project_path();
            if ( !std::filesystem::is_directory(project_path) )
                project_path.remove_filename();
            std::filesystem::path temporary_directory = project_path / "tmp";

            if ( std::filesystem::exists(temporary_directory) && !std::filesystem::is_directory(temporary_directory) )
                std::filesystem::remove(temporary_directory);

            if ( !std::filesystem::is_directory(temporary_directory) )
                std::filesystem::create_directory(temporary_directory);

            return temporary_directory;
        }
    }

    std::filesystem::path get_tmp_path_from_uid(const std::string& uid) {
        assert(!uid.empty());
        std::filesystem::path file_path = get_project_temporary_directory() / uid;
        file_path.replace_extension(extension);

        return file_path;
    }

    void delete_temporary_file(const std::string& uid) {
        const std::filesystem::path file_path = get_tmp_path_from_uid(uid);

        std::filesystem::remove(file_path);
    }

    void delete_temporary_files() {

        std::filesystem::path tmp_dir = get_project_temporary_directory();

        for ( const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(tmp_dir) ) {
            auto& path = entry.path();
            if ( path.extension() == extension ) {
                std::cout << "Deleting tmp file with path: " << path << std::endl;
                std::filesystem::remove(path);
            }
        }
    }



}