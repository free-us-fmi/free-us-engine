#include "material_view.h"
#include "imgui.h"
#include "managers/MaterialManager.h"
#include "new_material_popup.h"
#include "assets/selected_asset_view.h"
namespace editor::materials 
{

namespace {
	std::string _selected_material_id;
	bool popup_active = false;
}


std::string get_selected_material_id()
{
	return _selected_material_id;
}

void update()
{
	auto& materials = ::materials::get_materials();
	

	for ( auto& material : materials )
	{
		std::string itemid = "##" + material._uid;
		if ( ImGui::Selectable(itemid.c_str(), material._name == _selected_material_id) )
		{
			selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::materials);
			_selected_material_id = material._name;
		}
		ImGui::SameLine();
		ImGui::Text("%s", material._name.c_str());
	}

	if ( ImGui::Button("Add material") )
	{
		popup_active = true;	
		popup::initialize();
		popup::open();
	}

	if ( popup_active )
		popup::update();

	if ( popup::finished() and popup_active )
	{
		popup_active = false;
		::materials::AddMaterial(popup::result());
	}

}


}
