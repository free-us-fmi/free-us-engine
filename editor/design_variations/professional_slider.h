#pragma once
#include "imgui.h"

namespace editor::professional_slider
{
    void ProfessionalSlider(const char* label, float* value, float min_val, float max_val, const char* unit = "");
}