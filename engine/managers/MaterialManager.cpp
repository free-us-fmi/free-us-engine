#include "MaterialManager.h"
#include "TextureManager.h"
#include "utility/uid.h"

namespace materials 
{

namespace {
	utl::vector<material, false> materials;
	std::unordered_map<std::string, material_id> materials_map;
}

bool exists(material_id id) {
	assert(id < materials.size());
	return !materials.is_tombstone(materials.internal_begin() + id);
}

utl::vector<material, false>& get_materials()
{
	return materials;
}

material_id GetMaterialId(const std::string& material_name) {
	return materials_map[material_name];
}

material_id AddMaterial(const std::string& name)
{
	return AddMaterial(name, utl::uid::get_prefix_uid("mat"));
}

material_id AddMaterial(const std::string& name, const std::string& uid)
{
	if (materials_map.contains(name))
		return materials_map[name];

	material_id id = materials.emplace_tombstone();
	materials[id]._name = name;
	materials[id]._uid = uid;
	materials_map[name] = id;
	return id;
}

material* GetMaterial(material_id id)
{
	assert(id < materials.size());
	return &materials[id];

}

material* GetMaterial(const std::string& name)
{
	assert( materials_map.contains(name));
	return GetMaterial(materials_map[name]);
}

void RemoveMaterial(material_id id) {
	assert(id < materials.size());
	if ( !exists(id) )
		return;

	for ( unsigned int i = 0; i < material::texture_type::count; ++i )
		change_texture(material::texture_type(i), id, std::filesystem::path());
	materials_map.erase(materials[id]._name);
	materials.erase(materials.internal_begin() + id);
}

void RemoveMaterial(const std::string& name) {
	RemoveMaterial(GetMaterialId(name));
}

void UnloadMaterials() {
	utl::vector<material_id> ids;
	for ( auto& material : materials_map )
		ids.emplace_back(material.second);
	for ( auto id : ids )
		RemoveMaterial(id);
}

void change_texture(material::texture_type type, material_id id, const std::filesystem::path& path)
{
	assert(id < materials.size());
	material* material = GetMaterial(id);

	material->set_texture(type, path);
}

}
