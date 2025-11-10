#include "assets.h"

#include "managers/TextureManager.h"
#include "utility/EditorPath.h"
#include <map>
namespace editor::assets::browser_assets {
    namespace {
        std::map<asset, textures::texture_id> textures;
    }
    void initialize() {
        textures[asset::directory] = ::textures::add_texture(editor::GetEditorPath() + "internal_assets/folder.png");
        textures[asset::file] = ::textures::add_texture(editor::GetEditorPath() + "internal_assets/file.png");
    }
    textures::texture_id get_texture_id(asset asset) {
        return textures[asset];
    }
}
