#pragma once 
#include <filesystem>

#include "core/common.h"
#include "utility/vector.h"
#include "materials/materials.h"
#include <unordered_map>
#include <string>

namespace materials 
{
	material_id AddMaterial(const std::string& name);
	material_id AddMaterial(const std::string& name, const std::string& uid);

	material_id GetMaterialId(const std::string& material_name);
	material* GetMaterial(material_id id);	
	material* GetMaterial(const std::string& name);

	void RemoveMaterial(material_id id);
	void RemoveMaterial(const std::string& name);
	void UnloadMaterials();

	utl::vector<material, false>& get_materials();

	void change_texture(material::texture_type type, material_id id, const std::filesystem::path& texture_name);

}
