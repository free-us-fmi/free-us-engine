#pragma once
#include <string>

#include "content/scene.h"
#include "core/serializer.h"

namespace serializer::content::scene {
    void save(writer& main_writer);

    void load_scene(const std::string& scene_uid);
}
