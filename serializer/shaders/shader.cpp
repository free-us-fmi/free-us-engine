#include "shader.h"
#include <filesystem>

#include "project/active_project.h"
#include "engine/managers/ProgramManager.h"
#include "engine/managers/ShaderManager.h"
#include "shaders/program.h"
#include "shaders/src/ShadersPath.h"

namespace serializer::shaders {


    std::filesystem::path get_shaders_path() {
        return project::active::get_project_path() / "shaders";
    }

    std::filesystem::path get_shader_path(const std::string& shader_name) {
        assert(!shader_name.empty());
        std::filesystem::path scene_path =  get_shaders_path() / shader_name;

        return scene_path;
    }

    void create_shaders_directory() {
        std::filesystem::path shaders_path = get_shaders_path();

        if ( !std::filesystem::is_directory(shaders_path) )
            std::filesystem::remove(shaders_path);

        if ( !std::filesystem::exists(shaders_path) )
            std::filesystem::create_directory(shaders_path);
    }

    void load_shader(const std::filesystem::path& shader_path) {
        std::string shader_extension = shader_path.extension().string();

        if ( shader_extension != ".fs" )
            return;

        ::programs::program_id prog = programs::AddProgram(shader_path.filename().string(), ::shaders::GetShadersPath() + "raymarching/raymarch.vs",
                                       shader_path.string());
        ::programs::program* program = ::programs::GetProgram(prog);
        program->_editor_visible = true;
    }

    void create_shader(const std::string& shader_name) {
        create_shaders_directory();
        std::filesystem::path shader_path = get_shader_path(shader_name);
        if ( !std::filesystem::exists(shader_path) ) {
            std::ofstream shader_file(shader_path);
            shader_file.close();
            load_shader(shader_path);
        }
    }

    void load() {
        for ( const auto& entry : std::filesystem::recursive_directory_iterator(get_shaders_path())) {
            std::filesystem::path shader_path = entry.path();
            load_shader(shader_path);
        }
    }

}
