#pragma once 
#include "mesh.h"
#include "core/GLCommon.h"
#include "shaders/program.h"

namespace content::model 
{
	
struct model 
{
	utl::vector<unsigned int> _meshes;
	glm::mat4 _local_model;

	void draw(programs::program* prog,const glm::mat4& global, bool transparent = false);
	
	void set_material(std::string mat);
	void remove_instance(ecs::entity::entity_id entity_id);
	void instantiate(ecs::entity::entity_id entity_id);
};

model* get_model(unsigned int id);
unsigned int add_model(const model& m);
void remove_model(unsigned int id);

}

