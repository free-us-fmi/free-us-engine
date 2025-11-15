#include "transform_view.h"
#include "ECS/entity_view.h"
#include "editor/editor_common.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "editor/design_variations/professional_style.h"
#include "editor/design_variations/professional_slider.h"
#include "design_variations/professional_slider.h"
#include "design_variations/professional_style.h"

namespace editor::transform
{

static bool show_position_controls = false;
static bool show_rotation_controls = false;
static bool show_scale_controls = false;
static bool show_size_controls = false;

void update(ecs::components::transform::transform* transform, const std::string& entity_name)
{
    if (transform == nullptr)
        return;

    ImGui::BeginGroup();

    //apply ps
    editor::professional_style::ApplyProfessionalStyle();

    ImGui::Spacing();
    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "TRANSFORM");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(entity_name.c_str()).x);
    ImGui::TextDisabled("%s", entity_name.c_str());

    ImGui::Separator();

    //position section
    if (ImGui::Selectable("Position", show_position_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_position_controls = !show_position_controls;
    }

    if (show_position_controls)
    {
        glm::vec3 pos = transform->get_position();

        ImGui::Spacing();
        ImGui::Spacing();

        editor::professional_slider::ProfessionalSlider("X", &pos.x, -100.0f, 100.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("Y", &pos.y, -100.0f, 100.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("Z", &pos.z, -100.0f, 100.0f);
        ImGui::Spacing();

        transform->set_position(pos);

    }

    ImGui::Separator();

    //stretch section
    if (ImGui::Selectable("Stretch", show_scale_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_scale_controls = !show_scale_controls;
    }

    if (show_scale_controls)
    {
        glm::vec3 scale = transform->get_scale();

        ImGui::Spacing();
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("Length", &scale.x, 0.1f, 10.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("Height", &scale.y, 0.1f, 10.0f);
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("Width", &scale.z, 0.1f, 10.0f);
        ImGui::Spacing();

        transform->set_scale(scale);

    }

    ImGui::Separator();


    //size section
    if (ImGui::Selectable("Size", show_size_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_size_controls = !show_size_controls;
    }

    if (show_size_controls)
    {
        glm::vec3 scale = transform->get_scale();

        float avg_scale = (scale.x + scale.y + scale.z) / 3.0f;
        float min_scale = 0.0001f;
        float max_scale = 10.0f;

        if (avg_scale < min_scale || avg_scale > max_scale)
        {
            avg_scale = (min_scale + max_scale) / 2.0f;
        }

        float unified_scale = avg_scale;

        ImGui::Spacing();
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("Size", &unified_scale, min_scale, max_scale);

        if (unified_scale != avg_scale)
        {
            transform->set_scale(glm::vec3(unified_scale, unified_scale, unified_scale));
        }
        ImGui::Spacing();
    }

    ImGui::Separator();

    //rotation section
    if (ImGui::Selectable("Rotation", show_rotation_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_rotation_controls = !show_rotation_controls;
    }

    if (show_rotation_controls)
    {
        glm::vec3 rotation = transform->get_rotation();

        float min_rot = 3 * (-3600.0f);
        float max_rot = 3 * 3600.0f;

        float rot_x = glm::degrees(rotation.x);
        float rot_y = glm::degrees(rotation.y);
        float rot_z = glm::degrees(rotation.z);

        ImGui::Spacing();
        ImGui::Spacing();

        editor::professional_slider::ProfessionalSlider("OX", &rot_x, min_rot, max_rot, "°");
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("OY", &rot_y, min_rot, max_rot, "°");
        ImGui::Spacing();
        editor::professional_slider::ProfessionalSlider("OZ", &rot_z, min_rot, max_rot, "°");
        ImGui::Spacing();

        if (rot_x != glm::degrees(rotation.x) || rot_y != glm::degrees(rotation.y) || rot_z != glm::degrees(rotation.z))
        {
            glm::vec3 new_rot = glm::vec3(glm::radians(rot_x), glm::radians(rot_y), glm::radians(rot_z));
            transform->set_rotation(new_rot);
        }

    }

    ImGui::Separator();

    //restore original style
    editor::professional_style::RestoreStyle();
    ImGui::EndGroup();
}

}
