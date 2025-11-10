#include "entity_view.h"
#include "editor/editor_common.h"
#include "ECS/ecs.h"
#include "assets/selected_asset_view.h"
#include "assimp/contrib/earcut-hpp/earcut.hpp"
#include "utility/helpers/draw_browser.h"
#include "utility/helpers/textbox/textbox_popup.h"

namespace editor::entity 
{

namespace {
	ecs::entity::entity_id _selected_entity_id{id::invalid_id};
}

ecs::entity::entity_id get_selected_entity()
{
	return _selected_entity_id;
}

void update()
{
	static bool creation_popup = false;

	auto& entities = ecs::get_entity_vector();

	ImGui::Begin("entities");
	for ( auto& entity : entities )
	{
		std::string itemid = "##" + entity.get_name() + "_" + std::to_string(entity.get_id());
		if ( ImGui::Selectable(itemid.c_str(), entity.get_id() == _selected_entity_id) )
		{
			_selected_entity_id = entity.get_id();
			selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::entities);
		}
		ImGui::SameLine();
		ImGui::Text("%s", entity.get_name().c_str());
	}

	if ( ImGui::Button("Add entity") )
	{
		helpers::textbox::popup::initialize("entity name", "add entity");
		helpers::textbox::popup::open();
		creation_popup = true;
	}

	helpers::textbox::popup::update();

	if ( creation_popup && helpers::textbox::popup::finished() ) {
		creation_popup = false;
		ecs::create_entity(helpers::textbox::popup::result());
	}


	ImGui::End();

}

}
