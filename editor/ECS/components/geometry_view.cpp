#include "geometry_view.h"
#include "ECS/ecs.h"
#include "imgui.h"
#include "content/model_popup.h"
#include <iostream>
namespace editor::geometry 
{

void update(ecs::entity::entity_id entity_id)
{
	static bool popup = false;
	static std::string model_path = "";
	ecs::entity::entity* entity = ecs::get_entity(entity_id);

	ImGui::BeginGroup();
	if ( ecs::components::geometry::is_valid(entity->get_geometry_id()) )
	{
		ecs::components::geometry::geometry* geometry = entity->get_geometry();
		ImGui::Text("geometry");
		ImGui::Text("%s", geometry->get_model_name().c_str());
		
		if ( ImGui::Button("Change model") )
		{
			model_browser::popup::initialize();
			model_browser::popup::open();
			popup = true;
		}

			
		if ( model_path != "" )
		{
			ImGui::CloseCurrentPopup();
			entity->remove_geometry();
			entity->create_geometry(model_path, model_browser::popup::program_result());
			model_path = "";
			popup = false;
		}
	}
	else if ( ecs::components::instanced_geometry::is_valid(entity->get_instanced_geometry_id()) )
	{
		ecs::components::instanced_geometry::instanced_geometry* geometry = entity->get_instanced_geometry();
		ImGui::Text("instanced geometry");
		ImGui::Text("%s", geometry->get_model_name().c_str());
		if (ImGui::Button("Change model") )
		{

			model_browser::popup::initialize();
			model_browser::popup::open();
			popup = true;
		}
		if ( model_path != "" )
		{
			ImGui::CloseCurrentPopup();
			entity->remove_instanced_geometry();
			entity->create_instanced_geometry(model_path);
			model_path = "";
			popup = false;
		}
	}
	
	
	if ( popup)
	{
		model_browser::popup::update();
		model_path = model_browser::popup::result();
	}
	


	ImGui::EndGroup();
}

}
