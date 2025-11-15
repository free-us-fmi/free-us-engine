#include "materials.h"

#include "assimp/code/AssetLib/Blender/BlenderScene.h"
#include "managers/TextureManager.h"
#include "shaders/program.h"
#include "managers/ProgramManager.h"

namespace materials {
    std::array<std::string, material::texture_type::count> material::type_name = { "diffuse", "specular" };

    bool material::has_texture(texture_type type) const {
        return textures.contains(type);
    }

    void material::remove_texture(texture_type type) {
        if ( !has_texture(type) )
            return;
        textures::remove_texture(textures[type]);
        textures.erase(type);
    }

    void material::set_texture(texture_type type, const std::filesystem::path& path) {
        remove_texture(type);
        if ( !path.empty() )
            textures[type] = textures::add_texture(path);
    }

    void material::set_texture_from_data(const std::string& uid, texture_type type, unsigned char* data, unsigned int width, unsigned int height, unsigned int channels) {
        remove_texture(type);
        if ( !data ) return;
        textures[type] = textures::add_empty_texture();
        ::textures::texture_2d* tex = ::textures::get_texture(textures[type]);
        tex->initialize_from_data(uid, data, width, height, channels);
    }

    void material::bind_uniforms(texture_type type, programs::program* prog) {

        prog->SetUniform1i("material.has_" + type_name[type], has_texture(type));

        if ( !has_texture(type) )
            return;

        unsigned int tex_slot = textures::bind_texture(textures[type]);
        prog->SetUniform1i("material." + type_name[type], tex_slot);
    }

    void material::unbind_uniforms(texture_type type) {
        if ( !has_texture(type) )
            return;
        textures::unbind_one(textures[type]);
    }
}
