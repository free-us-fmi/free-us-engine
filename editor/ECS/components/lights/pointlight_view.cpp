#include "pointlight_view.h"
#include "imgui.h"
#include "editor/design_variations/professional_style.h"
#include "editor/design_variations/professional_slider.h"

namespace editor::pointlight
{

static bool show_ambient_controls = false;
static bool show_diffuse_controls = false;
static bool show_specular_controls = false;

void update(ecs::components::point_light::point_light* point_light)
{
    if (point_light == nullptr)
        return;

    ImGui::BeginGroup();

    //apply ps
    editor::professional_style::ApplyProfessionalStyle();

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "POINT LIGHT");
    ImGui::Separator();

    //ambient section
    if (ImGui::Selectable("Ambient", show_ambient_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_ambient_controls = !show_ambient_controls;
    }

    if (show_ambient_controls)
    {
        glm::vec3 ambient = point_light->get_ambient();

        ImGui::Spacing();
        ImGui::Spacing();

        editor::professional_slider::ProfessionalSlider("AR", &ambient.x, 0.0f, 4.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("AG", &ambient.y, 0.0f, 4.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("AB", &ambient.z, 0.0f, 4.0f);
        ImGui::Spacing();

        point_light->set_ambient(ambient);
    }

    ImGui::Separator();

    //diffuse section
    if (ImGui::Selectable("Diffuse", show_diffuse_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_diffuse_controls = !show_diffuse_controls;
    }

    if (show_diffuse_controls)
    {
        glm::vec3 diffuse = point_light->get_diffuse();

        ImGui::Spacing();
        ImGui::Spacing();

        editor::professional_slider::ProfessionalSlider("DR", &diffuse.x, 0.0f, 4.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("DG", &diffuse.y, 0.0f, 4.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("DB", &diffuse.z, 0.0f, 4.0f);
        ImGui::Spacing();

        point_light->set_diffuse(diffuse);
    }

    ImGui::Separator();

    //specular section
    if (ImGui::Selectable("Specular", show_specular_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_specular_controls = !show_specular_controls;
    }

    if (show_specular_controls)
    {
        glm::vec3 specular = point_light->get_specular();

        ImGui::Spacing();
        ImGui::Spacing();

        editor::professional_slider::ProfessionalSlider("SR", &specular.x, 0.0f, 10.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("SG", &specular.y, 0.0f, 10.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("SB", &specular.z, 0.0f, 10.0f);
        ImGui::Spacing();

        point_light->set_specular(specular);
    }

    ImGui::Separator();

    //restore og style
    editor::professional_style::RestoreStyle();
    ImGui::EndGroup();
}

}