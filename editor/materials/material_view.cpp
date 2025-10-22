#include "material_view.h"
#include "imgui.h"
#include "managers/MaterialManager.h"
#include "new_material_popup.h"
#include "assets/selected_asset_view.h"
namespace editor::materials 
{

namespace {
	::materials::material_id _selected_material_id{id::invalid_id};
	bool popup_active = false;
}


::materials::material_id get_selected_material_id()
{
	return _selected_material_id;
}

void update()
{
	auto materials = ::materials::get_materials();
	

	for ( auto& material : materials )
	{
		std::string itemid = "##" + material.first;
		if ( ImGui::Selectable(itemid.c_str(), material.second == _selected_material_id) )
		{
			selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::materials);
			_selected_material_id = material.second;
		}
		ImGui::SameLine();
		ImGui::Text("%s", material.first.c_str());
	}

	if ( ImGui::Button("Add material") )
	{
		popup_active = true;	
		popup::initialize();
		popup::open();
	}

	if ( popup_active )
		popup::update();

	if ( popup::finsihed() and popup_active )
	{
		popup_active = false;
		::materials::AddMaterial(popup::result());
	}

}


}
