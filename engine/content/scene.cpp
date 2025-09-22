#include "scene.h"
#include <unordered_map>
#include "content_loader.h"
#include "utility/path.h"
#include <atomic>

namespace content::scene 
{

namespace {
	std::unordered_map<std::string, scene> scenes;
	std::mutex creation_mutex;
	std::atomic_bool _creation_started;
}

bool creation_started()
{
	return _creation_started.load();
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
	std::lock_guard<std::mutex> gl(creation_mutex);

	_creation_started.store(true);

	if ( scenes.find(scene_name) != scenes.end() )
	{
		_creation_started.store(false);
		return;
	}
	scenes[scene_name] = create_scene_from_file(path, uv_flipped);		
	_creation_started.store(false);
}

scene* get_scene(const std::string& path)
{
	assert(scenes.find(path) != scenes.end());
	return &scenes[path];
}

void scene::instantiate(ecs::entity::entity_id entity_id)
{
	for ( auto m : _models )
	{
		auto model = model::get_model(m);
		model->instantiate(entity_id);
	}
}

void scene::remove_instance(ecs::entity::entity_id entity_id)
{
	for ( auto m : _models )
	{
		auto model = model::get_model(m);
		model->remove_instance(entity_id);
	}
}

void instantiate(const std::string& path, ecs::entity::entity_id entity_id)
{
	scenes[path].instantiate(entity_id);
}

void remove_instance(const std::string& path, ecs::entity::entity_id entity_id)
{
	scenes[path].remove_instance(entity_id);
}
}
