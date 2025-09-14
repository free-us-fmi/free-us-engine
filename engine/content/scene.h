#pragma once
#include "model.h"
#include "utility/vector.h"
#include "shaders/program.h"
#include <unordered_map>
namespace content::scene 
{

struct scene 
{
	utl::vector<unsigned int> _models;	

	void draw(programs::program* prog, glm::mat4 global_model);
};
std::unordered_map<std::string, scene>& get_scenes();
void create_scene(const std::string& scene_name, std::string path, bool uv_flipped = true);
scene* get_scene(std::string path);
}
