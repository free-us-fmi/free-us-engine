#pragma once 

namespace editor::assets 
{

enum class selected_assets_tab 
{
	models,
	materials,
	shaders
};

selected_assets_tab get_selected_asset_type();

void update();
}
