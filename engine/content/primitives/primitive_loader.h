#pragma once 
#include <string>

namespace content::primitives 
{

enum class primitive_types 
{
	plane,
	cube,
};

struct primitive_data 
{
	float scale = 1.f;
	unsigned int divisions_x = 0;
	unsigned int divisions_z = 0;
};

primitive_data get_default_plane_data();

primitive_data get_default_cube_data();

void create_primitive(const std::string& name, primitive_types type,const primitive_data& creation_data);

}
