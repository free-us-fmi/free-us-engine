#include "selected_entity_view.h"
#include "../entity_view.h"
#include "ECS/ecs.h"
#include "editor/editor_common.h"
#include "editor/files/file_browser.h"
#include "transform_view.h"
#include "assets/AssetsPath.h"
#include "content/scene.h"
#include "editor/content/model_popup.h"

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

	static std::string model_path = "";

	if ( ImGui::Button("Add component") )
	{
		ImGui::SetNextWindowSize(ImVec2(256, 256));
		ImGui::OpenPopup("Add component");
		model_path = "";
	}
	
	static bool model_popup = false;

	if ( ImGui::BeginPopupModal("Add component") )
	{
		if ( ImGui::Button("geometry"))
		{
			ImGui::SetNextWindowSize(ImVec2(512, 512));
			model_browser::popup::initialize();		
			model_popup = true;
		}
		if ( model_popup)
			model_browser::popup::update();
		model_path = model_browser::popup::result();
		if ( model_path != "" )
		{
			std::cout << "debug " << model_path << std::endl;
			ImGui::CloseCurrentPopup();
			_entity->create_geometry(model_path, program_id);
		}

		ImGui::EndPopup();
	}





	ImGui::End();
}

}
