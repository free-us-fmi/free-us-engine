#include "selected_model_view.h"
#include "model_view.h"
#include "engine/content/scene.h"
#include "materials/material_popup.h"
#include "engine/content/scene.h"
#include "imgui.h"

namespace editor::selected_model
{

void update()
{
	static bool material_popup = false;

	std::string selected_model = model::get_selected_model();
	content::scene::scene* _scene = content::scene::get_scene(selected_model);

	if ( ImGui::Button("Change material", ImVec2(-1, 0)))
	{
		material_popup = true;
		material_browser::popup::initialize_and_open();
	}

	ImGui::Checkbox("Transparent", &_scene->transparent);

	if ( material_popup )
	{
		std::string material_result = material_browser::popup::update();

		if ( material_result != "" )
		{
			_scene->set_material(material_result);
			material_popup = false;
		}
	}
}

}
