#include "model_browser.h"
#include "content/scene.h"
#include "imgui.h"
#include "content/scene.h"
#include "editor/utility/EditorPath.h"
#include "editor/utility/helpers/draw_browser.h"
#include "utility/vector.h"

namespace editor::model_browser
{


void model_browser::update()
{
	std::unordered_map<std::string, content::scene::scene>& scenes = content::scene::get_scenes();
	utl::vector<std::string> scene_names;
	for ( auto& scene : scenes )
		scene_names.emplace_back(scene.first);
	string_browser::browser::update(scene_names);
}


}
