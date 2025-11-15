#pragma once
#include "core/common.h"

namespace raymarching {
    void initialize();
    void render();
    void set_program(std::string name);
    std::string get_program();
}