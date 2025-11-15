#include "selected_asset_view.h"
#include "imgui.h"
#include "assets_view.h"

#include "materials/selected_material_view.h"
#include "ECS/components/selected_entity_view.h"
#include "content/selected_model_view.h"

namespace editor::selected_asset 
{

namespace 
{
	last_selected_asset_type selected_asset_type = last_selected_asset_type::none;
}

void set_selected_asset_type(last_selected_asset_type type)
{
	selected_asset_type = type;
}

void update()
{
	ImGui::Begin("Properties");
	if (selected_asset_type == last_selected_asset_type::entities )
		selected_entity::update();
	else if ( selected_asset_type == last_selected_asset_type::materials )
		selected_material::update();
	else if ( selected_asset_type == last_selected_asset_type::models )
		selected_model::update();

	ImGui::End();
}

}
