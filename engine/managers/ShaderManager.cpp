#include "ShaderManager.h"

#include <filesystem>



namespace shaders {

    namespace {
        utl::vector<shader, false> shaders;
    }

    utl::vector<shader, false>& get_shaders() {
        return shaders;
    }

    bool exists(shader_id id) {
        assert(id < shaders.size());
        return !shaders.is_tombstone(shaders.internal_begin() + id);
    }

    shader_id add_shader(const std::filesystem::path& path, ShaderType type, bool editor_visible) {
        return shaders.emplace_tombstone(path.string(), type, editor_visible);
    }

    shader* get_shader(shader_id id) {
        assert(exists(id));
        return &shaders[id];
    }

    void remove_shader(shader_id id) {
        assert(exists(id));
        shaders.erase(shaders.internal_begin() + id);
    }

    void unload() {
        for ( unsigned int i = 0; i < shaders.size(); i++ ) {
            if ( !shaders.is_tombstone(shaders.internal_begin() + i) && shaders[i]._editor_visible )
                shaders.erase(shaders.internal_begin() + i);
        }
    }
}
