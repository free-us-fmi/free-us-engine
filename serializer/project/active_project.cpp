#include "active_project.h"
#include <string>
#include <filesystem>
#include "core/serializer.h"
#include "content/scene.h"
#include "file/temporary_files.h"
#include "serializer/content/mesh.h"
#include "serializer/content/scene.h"
#include "serializer/materials/materials.h"
#include "scene.h"

namespace serializer::project::active {


    namespace data{
        std::filesystem::path project_path;
        std::string project_name;
        std::string active_scene_name = "default_scene";
    }

    std::string get_active_scene() {
        return data::active_scene_name;
    }

    std::filesystem::path get_project_path() {
        return data::project_path;
    }

    void save() {
        if ( data::project_path.empty() or data::project_name.empty() )
            return;

        if ( !std::filesystem::exists(data::project_path) or !std::filesystem::is_directory(data::project_path) )
            std::filesystem::create_directory(data::project_path);

        writer writer;

        writer.open((data::project_path / "project.sor").string());

        writer.write(data::project_name);

        materials::save(writer);
        content::scene::save(writer);
        project_scene::save(writer);

        writer.flush();
        writer.close();
    }

    void save_as(const std::filesystem::path& path, const std::string& name) {
        save();

        data::project_name = name;
        data::project_path = path / data::project_name;

        if ( std::filesystem::is_directory(data::project_path) and !std::filesystem::is_empty(data::project_path) ) {
            std::cout << "Error! Cannot save project. Save path must be an empty directory!";
            return;
        }

        save();
    }

    void load(const std::filesystem::path& path) {
        data::project_path = path;
        data::project_path.remove_filename();

        reader project_reader;
        project_reader.open_and_store(path.string());

        project_reader.read(data::project_name);
        std::cout << "loading " << data::project_name << " " << data::project_name.size() << std::endl;

        materials::load(project_reader);

        unsigned int no_scenes;
        project_reader.read(no_scenes);
        std::cout << "loading " << no_scenes << " scenes" << std::endl;
        while ( no_scenes-- ) {
            std::string uid;
            project_reader.read(uid);
            content::scene::load_scene(uid);
        }

        project_scene::load(project_reader);

        file::temp::delete_temporary_files();
    }

}
