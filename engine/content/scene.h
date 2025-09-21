#pragma once
#include "model.h"
#include "utility/vector.h"
#include "shaders/program.h"
#include <unordered_map>
#include <mutex>

namespace content::scene 
{

struct scene 
{
	utl::vector<unsigned int> _models;	

	void draw(programs::program* prog, glm::mat4 global_model);
};

bool creation_started();
std::mutex& get_creation_mutex();
std::unordered_map<std::string, scene>& get_scenes();
void create_scene(const std::string& scene_name, std::string path, bool uv_flipped = false);
scene* get_scene(const std::string& path);

void instantiate(const std::string& path, unsigned int transform_id);
}
