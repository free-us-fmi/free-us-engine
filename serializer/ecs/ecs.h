#pragma once
#include "core/serializer.h"

namespace serializer::ecs {
    void save(writer& scene_writer);
    void load(reader& scene_reader);
}