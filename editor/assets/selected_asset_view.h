#pragma once 

namespace editor::selected_asset 
{

enum class last_selected_asset_type 
{
	models,
	materials,
	entities,
	shaders,
	none
};

void set_selected_asset_type(last_selected_asset_type type);
void update();

}
