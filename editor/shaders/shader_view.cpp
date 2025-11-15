#include "shader_view.h"
#include "imgui.h"
#include "utility/helpers/textbox/textbox_popup.h"
#include "engine/managers/ShaderManager.h"
#include "shader_browser.h"
#include "assets/selected_asset_view.h"
#include "raymarcher/renderer.h"
#include "serializer/shaders/shader.h"
namespace editor::shaders {

    namespace {
        shader_browser::shader_browser shader_browser;
        std::string selected_shader;
    }

    void update() {
        static bool popup = false;

        if ( ImGui::Button("add shader")) {
            popup = true;
            helpers::textbox::popup::initialize("shader name", "add shader");
            helpers::textbox::popup::open();
        }

        if ( popup && helpers::textbox::popup::finished() ) {
            serializer::shaders::create_shader(helpers::textbox::popup::result());
            popup = false;
        }

        if ( popup )
            helpers::textbox::popup::update();

        auto& shaders = ::shaders::get_shaders();

        shader_browser.update();

        if ( selected_shader != shader_browser.last_selected_item() )
        {
            selected_shader = shader_browser.last_selected_item();
            ::raymarching::set_program(selected_shader);
            selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::shaders);
        }
    }

    std::string get_selected_shader() {
        return selected_shader;
    }

}