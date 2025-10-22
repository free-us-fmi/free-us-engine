#pragma once 
#include "utility/vector.h"

namespace materials 
{

struct material 
{
	utl::vector<std::string> _textures_map;
	utl::vector<std::string> _specular_map; 

	bool editor_visible = true;
};

}
