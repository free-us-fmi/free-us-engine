#pragma once
#include "core/serializer.h"

namespace serializer::project::project_scene {
    void save(writer& main_writer);
    void load(reader& main_reader);
}