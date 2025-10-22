#pragma once 
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <string>
#include "content_structures.h"
#include "shaders/program.h"
#include <unordered_map>
#include "materials/materials.h"

namespace content::mesh 
{


struct mesh 
{
	utl::vector<glm::mat4> _transforms;
	std::unordered_map<ecs::entity::entity_id, unsigned int> entity_to_transform;
	std::unordered_map<unsigned int, ecs::entity::entity_id> tranform_to_entity;

	unsigned int _instance_buffer_capacity{0};

	utl::vector<vertex> _vertices;
	utl::vector<u32> _indices; 
	std::string _material = "";

	unsigned int _VBO;
	unsigned int _EBO;
	unsigned int _VAO;
	
	unsigned int _transform_buffer;

	void set_material(std::string mat);

	void draw(programs::program* prog, glm::mat4 model, bool transparent = false);
	void draw_instanced(programs::program* prog);
	void instantiate(ecs::entity::entity_id entity_id,const glm::mat4& _local_model);
	void remove_instance(ecs::entity::entity_id);
};

void render_instanced(programs::program* prog);
mesh* get_mesh(unsigned int id);
unsigned int add_mesh(mesh& m);
void remove_mesh(unsigned int id);

}
