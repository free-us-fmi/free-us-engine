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
#include "lights/pointlight_view.h"

namespace editor::selected_entity 
{

namespace {
	programs::program_id program_id;
}

void initialize(programs::program_id program)
{
	program_id = program;
}

void update()
{
	ImGui::Begin("entity view");	
	ecs::entity::entity_id id = entity::get_selected_entity();

	if ( id == id::invalid_id )
	{
		ImGui::End();
		return;
	}
	ecs::entity::entity* _entity = ecs::get_entity(id); 
	
	if ( _entity->get_transform() != nullptr )
		transform::update(_entity->get_transform());
	geometry::update(id, program_id);
	pointlight::update(_entity->get_point_light());

	static std::string model_path = "";
	static std::string component = "";

	if ( ImGui::Button("Add component") )
	{
		ImGui::SetNextWindowSize(ImVec2(256, 256));
		ImGui::OpenPopup("Add component");
		
		model_browser::popup::initialize();	
		model_path = "";
		component = "";
	}
	
	static bool model_popup = false;

	if ( ImGui::BeginPopupModal("Add component") )
	{
		if ( ImGui::Button("geometry"))
		{
			component = "geometry";
			model_popup = true;
			model_browser::popup::open();
		}

		if ( ImGui::Button("instanced geometry"))
		{
			component = "instanced geometry";
			model_popup = true;
			model_browser::popup::open();
		}

		if ( ImGui::Button("point light"))
		{
			component = "point light";
			_entity->create_point_light();
			ImGui::CloseCurrentPopup();
		}

		if ( model_popup)
			model_browser::popup::update();
		model_path = model_browser::popup::result();

		if ( model_path != "" )
		{
			ImGui::CloseCurrentPopup();
			if ( component == "geometry" )
				_entity->create_geometry(model_path, program_id);
			else _entity->create_instanced_geometry(model_path);
		}

		ImGui::EndPopup();
	}





	ImGui::End();
}

}
