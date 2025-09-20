#pragma once 
#include "core/GLCommon.h"
#include "utility/vector.h"
#include <string>
#include "content_structures.h"
#include "shaders/program.h"

namespace content::mesh 
{

struct material 
{
	utl::vector<std::string> _textures_map;
	utl::vector<std::string> _specular_map; 
};

struct mesh 
{
	utl::vector<glm::mat4> _transforms;

	utl::vector<vertex> _vertices;
	utl::vector<u32> _indices; 
	material _material;

	unsigned int _VBO;
	unsigned int _EBO;
	unsigned int _VAO;
	
	unsigned int _transform_buffer;

	void draw(programs::program* prog, glm::mat4 model);

	void remove_instance(unsigned int id);
	void draw_instanced(programs::program* prog);
	unsigned int instantiate(unsigned int transform_id);
};

void render_instanced(programs::program* prog);
mesh* get_mesh(unsigned int id);
unsigned int add_mesh(mesh& m);
void remove_mesh(unsigned int id);

}
