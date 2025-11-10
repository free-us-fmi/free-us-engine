#pragma once
#include "core/common.h"

namespace editor::assets::browser_assets {
    enum class asset {
        directory = 0,
        file
    };
    void initialize();
    textures::texture_id get_texture_id(asset asset);
}
