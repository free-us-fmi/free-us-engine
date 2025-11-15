#include "selected_entity_view.h"
#include "../entity_view.h"
#include "ECS/ecs.h"
#include "editor/editor_common.h"
#include "editor/files/file_browser.h"
#include "transform_view.h"
#include "assets/AssetsPath.h"
#include "content/scene.h"
#include "editor/content/model_popup.h"
#include "geometry_view.h"
#include "raymarched_geometry.h"
#include "lights/pointlight_view.h"
#include "engine/ECS/ecs.h"
#include "editor/design_variations/professional_style.h"

namespace editor::selected_entity
{

    static bool show_component_controls = false;
    static std::string pending_model_path = "";
    static std::string pending_component_type = "";
    static bool model_popup_active = false;
    static bool show_remove_confirmation = false;


void update()
{
    ecs::entity::entity_id id = entity::get_selected_entity();

    if (!ecs::is_valid(id))
        return;

    ecs::entity::entity* _entity = ecs::get_entity(id);

    if (_entity->get_transform() != nullptr)
        transform::update(_entity->get_transform(), _entity->get_name());
    geometry::update(id);
    pointlight::update(_entity->get_point_light());
    raymarched_geometry::update(id);
    //PS
    editor::professional_style::ApplyProfessionalStyle();

    ImGui::Spacing();

    //component management section
    if (ImGui::Selectable("Component Management", show_component_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
    {
        show_component_controls = !show_component_controls;
    }

    if (show_component_controls)
    {
        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Selectable("Add Component"))
        {
            ImGui::SetNextWindowSize(ImVec2(256, 256));
            ImGui::OpenPopup("Add component");
            model_browser::popup::initialize();
            pending_model_path = "";
            pending_component_type = "";
            model_popup_active = false; // RESET POPUP STATE
        }

        if (ImGui::Selectable("Remove Entity"))
        {
            show_remove_confirmation = true;
        }

        ImGui::Spacing();
    }

    ImGui::Separator();
    editor::professional_style::RestoreStyle();

    if (show_remove_confirmation)
    {
        ImGui::OpenPopup("Confirm Removal");
        show_remove_confirmation = false;
    }

    if (ImGui::BeginPopupModal("Confirm Removal", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Are you sure you want to remove this entity?");
        ImGui::Spacing();

        if (ImGui::Button("Confirm", ImVec2(150, 25)))
        {
            ecs::remove_entity(id);
            ImGui::CloseCurrentPopup();
            editor::professional_style::RestoreStyle();
            return;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(150, 25)))
        {
            ImGui::CloseCurrentPopup();
            model_popup_active = false;
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Add component"))
    {
        if (ImGui::Button("Geometry"))
        {
            pending_component_type = "geometry";
            model_popup_active = true;
            model_browser::popup::open();
        }

        if (ImGui::Button("Instanced geometry"))
        {
            pending_component_type = "instanced geometry";
            model_popup_active = true;
            model_browser::popup::open();
        }

        if (ImGui::Button("Point light"))
        {
            pending_component_type = "point light";
            _entity->create_point_light();
            pending_component_type = "";
            model_popup_active = false;
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("raymarched geometry"))
        {
            pending_component_type = "point light";
            _entity->create_raymarched_geometry();
            pending_component_type = "";
            model_popup_active = false;
            ImGui::CloseCurrentPopup();
        }

        if (model_popup_active)
        {
            model_browser::popup::update();
            pending_model_path = model_browser::popup::result();
        }

        if (!pending_model_path.empty())
        {
            ImGui::CloseCurrentPopup();

            if (pending_component_type == "geometry")
            {
                _entity->create_geometry(pending_model_path, model_browser::popup::program_result());
            }
            else if (pending_component_type == "instanced geometry")
            {
                _entity->create_instanced_geometry(pending_model_path);
            }

            pending_model_path = "";
            pending_component_type = "";
            model_popup_active = false;
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 85, ImGui::GetWindowSize().y - 35));
        if (ImGui::Button("Cancel", ImVec2(80, 25)))
        {
            pending_component_type = "";
            pending_model_path = "";
            model_popup_active = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

}