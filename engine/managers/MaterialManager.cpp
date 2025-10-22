#include "MaterialManager.h"
#include "TextureManager.h"

namespace materials 
{

namespace {
	utl::vector<material, false> materials;
	std::unordered_map<std::string, material_id> materials_map;
}

material_id AddMaterial(const std::string& name)
{
	assert( materials_map.find(name) == materials_map.end());
	material_id id = materials.emplace_tombstone();
	materials_map[name] = id;
	return id;
}

material_id AddMaterial(const std::string& name, const material& mat)
{
	material_id id = AddMaterial(name);
	materials[id] = mat;
	return id;
}

material* GetMaterial(material_id id)
{
	assert(id < materials.size());
	return &materials[id];

}

material* GetMaterial(const std::string& name)
{
	assert( materials_map.find(name) != materials_map.end() );
	return GetMaterial(materials_map[name]);
}

std::unordered_map<std::string, material_id>& get_materials()
{
	return materials_map;
}

void change_diffuse(material_id id, const std::string& texture_name)
{
	assert(id < materials.size());
	material* material = GetMaterial(id);
	material->_textures_map.clear();

	if ( !textures::exists(texture_name) )
		textures::add_texture(texture_name);

	material->_textures_map.emplace_back(texture_name);
}

void change_specular(material_id id, const std::string& texture_name)
{
	assert(id < materials.size());
	material* material = GetMaterial(id);
	material->_specular_map.clear();

	if ( !textures::exists(texture_name) )
		textures::add_texture(texture_name);

	material->_specular_map.emplace_back(texture_name);
}


}
