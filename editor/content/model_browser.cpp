#include "model_browser.h"
#include "content/scene.h"
#include "imgui.h"
#include "content/scene.h"
#include "editor/utility/EditorPath.h"
#include "editor/utility/helpers/draw_browser.h"
#include "utility/vector.h"

namespace editor::model_browser
{

void model_browser::initialize()
{
	_last_selected_model = "";
}

void model_browser::update()
{
	ImGui::Text("%s", "models");
	
	utl::vector<helpers::draw_browser::browser_element> elements;
	std::unordered_map<std::string, content::scene::scene>& scenes = content::scene::get_scenes();	

	for ( auto& entry : scenes )
	{	
		std::string tex = editor::GetEditorPath() + "assets/file.png";
		elements.push_back({entry.first, tex});	
	}
	std::string selected_model = helpers::draw_browser::draw_browser(elements);

	_last_selected_model = selected_model;
}

	
}
