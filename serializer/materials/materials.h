#pragma once
#include "core/serializer.h"

namespace serializer::materials {
    void save(writer& main_writer);
    void load(reader& main_reader);
}