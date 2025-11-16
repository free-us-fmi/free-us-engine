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
	std::string material_name = materials::get_selected_material_id();

	ImGui::Text("Diffuse: ");
	ImGui::SameLine();
	if ( ImGui::Button("Change diffuse") )
	{
		popup = texture_popup_type::diffuse;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);
		file_browser::popup::open();
	}

	ImGui::Text("Specular: ");
	ImGui::SameLine();
	if ( ImGui::Button("Change specular") )
	{
		popup = texture_popup_type::specular;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);
		file_browser::popup::open();
	}

	if ( popup != texture_popup_type::none )
		file_browser::popup::update();

	if ( popup == texture_popup_type::diffuse )
		if ( !file_browser::popup::get_selected_file_path().empty() )
		{
			::materials::change_texture(::materials::material::texture_type::diffuse, ::materials::GetMaterialId(material_name), file_browser::popup::get_selected_file_path());
			popup = texture_popup_type::none;
		}
	
	if ( popup == texture_popup_type::specular )
		if ( !file_browser::popup::get_selected_file_path().empty() )
		{
			::materials::change_texture(::materials::material::texture_type::specular, ::materials::GetMaterialId(material_name), file_browser::popup::get_selected_file_path());
			popup = texture_popup_type::none;
		}
}

}
