#pragma once
#include <string>

namespace editor::helpers::textbox::popup {
    void initialize(const std::string& preview_text,const std::string& popup_name);
    void update();
    std::string result();
    void open();
    bool finished();
}