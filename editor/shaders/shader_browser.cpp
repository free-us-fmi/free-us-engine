#include "shader_browser.h"
#include "content/scene.h"
#include "imgui.h"
#include "content/scene.h"
#include "editor/utility/EditorPath.h"
#include "editor/utility/helpers/draw_browser.h"
#include "managers/ProgramManager.h"
#include "managers/ShaderManager.h"
#include "utility/vector.h"

namespace editor::shader_browser
{
    namespace {
        std::string selected_program;
    }
    void shader_browser::update()
    {
        std::unordered_map<std::string, ::programs::program_id>& programs_ids = ::programs::get_programs();
        utl::vector<std::string> programs_names;
        for ( auto& program_id: programs_ids ) {
            auto* program = ::programs::GetProgram(program_id.second);
            if ( program->_editor_visible )
                programs_names.emplace_back(program->get_name());
        }

        string_browser::browser::update(programs_names);
        selected_program = string_browser::browser::last_selected_item();
    }
}
