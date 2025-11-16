#include "material_browser.h"

#include "imgui.h"
#include "engine/utility/vector.h"
#include "engine/managers/MaterialManager.h"

namespace editor::material_browser
{

void browser::update()
{
	utl::vector<std::string> material_names;
	utl::vector<::materials::material, false>& materials = materials::get_materials();
	
	for ( auto& material : materials )
		material_names.emplace_back(material._name);

	float available_height = ImGui::GetContentRegionAvail().y - 40; // Reserve space for button
	ImGui::BeginChild("MaterialBrowserList", ImVec2(0, available_height), true);
	{
		string_browser::browser::update(material_names);
	}
	ImGui::EndChild();
}

}
