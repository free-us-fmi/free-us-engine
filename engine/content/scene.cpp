#include "scene.h"
#include <unordered_map>
#include "content_loader.h"
#include "utility/path.h"
namespace content::scene 
{

namespace {
	std::unordered_map<std::string, scene> scenes;
}

std::unordered_map<std::string, scene>& get_scenes()
{
	return scenes;
}

void scene::draw(programs::program* prog, glm::mat4 global_model)
{
	unsigned int i = 0;
	for ( auto c : _models )
	{
		model::model* m = model::get_model(c);
		m->draw(prog, global_model);
	}
}

void create_scene(const std::string& scene_name, std::string path, bool uv_flipped)
{
	if ( scenes.find(path) != scenes.end() )
		return;
	scenes[scene_name] = create_scene_from_file(path, uv_flipped);		
}

scene* get_scene(std::string path)
{
	return &scenes[path];
}


}
