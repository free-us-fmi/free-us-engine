#pragma once 
#include <string>
#include "core/common.h"
#include "utility/vector.h"
#include "imgui.h"
#include "managers/TextureManager.h"
namespace editor::helpers::draw_browser 
{
	
struct browser_element 
{
	std::string name;
	std::string icon_texture;
};

std::string draw_browser(utl::vector<browser_element>& elements);

}
