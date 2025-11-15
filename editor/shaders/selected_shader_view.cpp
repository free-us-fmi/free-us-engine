#include "selected_shader_view.h"

#include <string>

#include "imgui.h"
#include "shader_view.h"
#include "managers/ProgramManager.h"
#include "shaders/program.h"

namespace editor::selected_shader {
    void update() {
        std::string selected_name = shaders::get_selected_shader();
        auto* prog = ::programs::GetProgram(selected_name);

        ImGui::Text("%s", selected_name.c_str());
        if ( ImGui::Button("compile shader") ) {
            prog->Link();
        }
    }
}
