#include "imgui.h"
#include "ECS/ecs.h"

namespace editor::raymarched_geometry
{
    static bool show_raymarched_geometry_controls = false;
    void update(ecs::entity::entity_id entity_id) {



        static const char* items[] = {
            "ellipsoid",
            "rectangular_prism",
            "elliptic_cone",
            "elliptic_capped_cylinder",
            "pyramid",
            "tetrahedron",
        };


        ::ecs::components::raymarched_geometry::raymarched_geometry* geo = ::ecs::get_entity(entity_id)->get_raymarched_geometry();
        if ( !geo )
            return;

        if (ImGui::Selectable("Raymarched geometry", show_raymarched_geometry_controls, ImGuiSelectableFlags_None, ImVec2(0, 15)))
        {
            show_raymarched_geometry_controls = !show_raymarched_geometry_controls;
        }

        if ( !show_raymarched_geometry_controls )
            return;
        int selected_item = static_cast<int>(geo->get_model_type());

        if (ImGui::Combo("Model type", &selected_item, items, IM_ARRAYSIZE(items)))
            geo->set_model_type(static_cast<::ecs::components::raymarched_geometry::model_types>(selected_item));

    }
}
