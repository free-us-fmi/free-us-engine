#include "selected_material_view.h"
#include "imgui.h"
#include "files/file_popup.h"
#include "engine/assets/AssetsPath.h"
#include "material_view.h"
#include "engine/managers/MaterialManager.h"

namespace editor::selected_material 
{

enum class texture_popup_type
{
	diffuse,
	specular,

	none
};

namespace 
{
	texture_popup_type popup = texture_popup_type::none;
}

void update()
{
	::materials::material_id material_id = materials::get_selected_material_id();

	ImGui::Text("Diffuse: ");
	ImGui::SameLine();
	if ( ImGui::Button("change diffuse") )
	{
		popup = texture_popup_type::diffuse;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);
		file_browser::popup::open();
	}

	ImGui::Text("Specular: ");
	ImGui::SameLine();
	if ( ImGui::Button("change specular") )
	{
		popup = texture_popup_type::specular;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);
		file_browser::popup::open();
	}

	if ( popup != texture_popup_type::none )
		file_browser::popup::update();

	if ( popup == texture_popup_type::diffuse )
		if ( file_browser::popup::get_selected_file_path() != "" )
		{
			::materials::change_diffuse(material_id, file_browser::popup::get_selected_file_path());
			popup = texture_popup_type::none;
		}
	
	if ( popup == texture_popup_type::specular )
		if ( file_browser::popup::get_selected_file_path() != "" )
		{
			::materials::change_specular(material_id, file_browser::popup::get_selected_file_path());
			popup = texture_popup_type::none;
		}
}

}
