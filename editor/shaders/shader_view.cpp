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
        unsigned int browser_fence = 0;
    }

    void update()
    {
        static bool popup = false;

        ImGui::Spacing();

        // Professional button styling
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent black background
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.2f)); // Dark gray on hover
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // Gray when pressed
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5, 0.5, 0.5, 0.5)); // White border
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f); // Border thickness

        if ( ImGui::Button("Add shader", ImVec2(300, 0))) {
            popup = true;
            helpers::textbox::popup::initialize("Shader name", "Add shader");
            helpers::textbox::popup::open();
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(4);

        ImGui::Spacing();

        if ( popup )
            helpers::textbox::popup::update();

        if ( popup && helpers::textbox::popup::finished() ) {
            std::string shader_name = helpers::textbox::popup::result();
            if (!shader_name.empty()) {
                serializer::shaders::create_shader(shader_name);
            }
            popup = false;
        }

        auto& shaders = ::shaders::get_shaders();

        float available_height = ImGui::GetContentRegionAvail().y;
        ImGui::BeginChild("ShaderList", ImVec2(0, available_height), true);
        {
            ImGui::Spacing();
            shader_browser.update();

            if (browser_fence != shader_browser.get_fence())
            {
                browser_fence = shader_browser.get_fence();
                selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::shaders);
            }

            if ( selected_shader != shader_browser.last_selected_item() )
            {
                selected_shader = shader_browser.last_selected_item();
                ::raymarching::set_program(selected_shader);
                selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::shaders);
            }
        }
        ImGui::EndChild();
    }

    std::string get_selected_shader() {
        return selected_shader;
    }
}