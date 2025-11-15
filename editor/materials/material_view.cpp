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
	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent black background
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.2f)); // Dark gray on hover
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // Gray when pressed
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5, 0.5, 0.5, 0.5)); // White border
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f); // Border thickness

	if ( ImGui::Button("Add material", ImVec2(300, 0)) )
	{
		popup_active = true;
		popup::initialize();
		popup::open();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(4);

	ImGui::Spacing();

	auto& materials = ::materials::get_materials();
	
	float available_height = ImGui::GetContentRegionAvail().y;
	ImGui::BeginChild("MaterialList", ImVec2(0, available_height), true);
	{
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
	}
	ImGui::EndChild();

	if ( popup_active )
		popup::update();

	if ( popup::finished() and popup_active )
	{
		popup_active = false;
		::materials::AddMaterial(popup::result());
	}

}


}
