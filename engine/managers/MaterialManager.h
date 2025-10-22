#pragma once 
#include "core/common.h"
#include "materials/materials.h"
#include <unordered_map>
#include <string>

namespace materials 
{
	material_id AddMaterial(const std::string& name);
	material_id AddMaterial(const std::string& name, const material& mat);

	material* GetMaterial(material_id id);	
	material* GetMaterial(const std::string& name);	
	std::unordered_map<std::string, material_id>& get_materials();

	void change_diffuse(material_id id, const std::string& texture_name);
	void change_specular(material_id id, const std::string& texture_name);

}
