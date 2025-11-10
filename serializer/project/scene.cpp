#include "scene.h"

#include <cassert>
#include <filesystem>

#include "active_project.h"
#include "core/serializer.h"

#include "ecs/ecs.h"

namespace serializer::project::project_scene {

    constexpr const char* scene_extension = "scn";

    std::filesystem::path get_scenes_path() {
        return project::active::get_project_path() / "scenes";
    }

    std::filesystem::path get_scene_path(const std::string& scene_name, const char* extension = scene_extension) {
        assert(!scene_name.empty());
        std::filesystem::path scene_path =  get_scenes_path() / scene_name;
        scene_path.replace_extension(extension);

        return scene_path;
    }

    std::filesystem::path get_active_scene_path() {
        return get_scene_path(project::active::get_active_scene());
    }

    void create_scene_directory() {
        std::filesystem::path scenes_path = get_scenes_path();

        if ( !std::filesystem::is_directory(scenes_path) )
            std::filesystem::remove(scenes_path);

        if ( !std::filesystem::exists(scenes_path) )
            std::filesystem::create_directory(scenes_path);
    }

    void save(writer& main_writer) {
        create_scene_directory();
        auto active_scene_path = get_active_scene_path();

        writer scene_writer;
        scene_writer.open(active_scene_path.string());

        ecs::save(scene_writer);

        scene_writer.flush();
        scene_writer.close();
    }

    void load(reader& main_reader) {
        auto active_scene_path = get_active_scene_path();

        reader scene_reader;
        scene_reader.open_and_store(active_scene_path.string());

        ecs::load(scene_reader);
    }
}
