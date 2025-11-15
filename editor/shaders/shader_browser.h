#pragma once
#include <string>
#include "utility/helpers/string_browser.h"

namespace editor::shader_browser
{

    class shader_browser : public string_browser::browser
    {
    public:
        void update();
    };
}
