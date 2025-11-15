#include "geometry_view.h"
#include "ECS/ecs.h"
#include "imgui.h"
#include "content/model_popup.h"
#include "editor/design_variations/professional_style.h"
#include <iostream>

namespace editor::geometry
{

//statics
static bool popup_active = false;
static std::string pending_model_path = "";
static bool pending_is_instanced = false;
static bool show_geometry_section = false;
static bool show_instanced_section = false;

void update(ecs::entity::entity_id entity_id)
{
    if (!ecs::is_valid(entity_id))
        return;

    ecs::entity::entity* entity = ecs::get_entity(entity_id);

    //PS
    editor::professional_style::ApplyProfessionalStyle();
    ImGui::BeginGroup();

    bool has_geometry = ecs::components::geometry::is_valid(entity->get_geometry_id());
    bool has_instanced = ecs::components::instanced_geometry::is_valid(entity->get_instanced_geometry_id());

    //geometry section
    if (has_geometry)
    {
        if (ImGui::Selectable("Geometry Options", show_geometry_section, ImGuiSelectableFlags_None, ImVec2(0, 15)))
        {
            show_geometry_section = !show_geometry_section;
        }

        if (show_geometry_section)
        {
            ImGui::Spacing();
            ImGui::Spacing();

            if (ImGui::Selectable("Change Model"))
            {
                model_browser::popup::initialize();
                model_browser::popup::open();
                popup_active = true;
                pending_model_path = "";
                pending_is_instanced = false;
            }
        }
        ImGui::Separator();
    }
    //instanced geometry section
    else if (has_instanced)
    {
        if (ImGui::Selectable("Instanced Geometry Options", show_instanced_section, ImGuiSelectableFlags_None, ImVec2(0, 15)))
        {
            show_instanced_section = !show_instanced_section;
        }

        if (show_instanced_section)
        {
            ImGui::Spacing();
            ImGui::Spacing();

            if (ImGui::Selectable("Change Model"))
            {
                model_browser::popup::initialize();
                model_browser::popup::open();
                popup_active = true;
                pending_model_path = "";
                pending_is_instanced = true;
            }
        }
        ImGui::Separator();
    }

    ImGui::EndGroup();
    editor::professional_style::RestoreStyle();

    if (popup_active)
    {
        model_browser::popup::update();
        pending_model_path = model_browser::popup::result();

        if (!pending_model_path.empty())
        {
            ImGui::CloseCurrentPopup();

            if (!pending_is_instanced)
            {

                if (ecs::components::geometry::is_valid(entity->get_geometry_id()))
                {
                    entity->remove_geometry();
                    entity->create_geometry(pending_model_path, model_browser::popup::program_result());
                }

            }
            else
            {
                if (ecs::components::instanced_geometry::is_valid(entity->get_instanced_geometry_id()))
                {
                    entity->remove_instanced_geometry();
                    entity->create_instanced_geometry(pending_model_path);
                }

            }

            pending_model_path = "";
            popup_active = false;
        }
    }
}

}