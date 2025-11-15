#pragma once
#include <filesystem>

#include "shaders/shader.h"
#include "utility/vector.h"
#include "core/common.h"

namespace shaders {
    utl::vector<shader, false>& get_shaders();

    shader_id add_shader(const std::filesystem::path& path, ShaderType type, bool editor_visible = false);
    shader* get_shader(shader_id id);
    void remove_shader(shader_id id);
    void unload();
}
