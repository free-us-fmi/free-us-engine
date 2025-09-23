#include "instanced_geometry.h"
#include "content/scene.h"
#include "ECS/ecs.h"
#include "utility/assimp_glm.h"
namespace ecs::components::instanced_geometry 
{

namespace {
	utl::vector<instanced_geometry, false, 1024> components;
	utl::vector<unsigned int> generations;
}

instanced_geometry::instanced_geometry(entity::entity_id entity, const std::string& model)
{
	_model = model;
	_entity_id = entity;
	_model_mat = get_entity(entity)->get_transform()->get_model();
}

void instanced_geometry::instantiate()
{
	assert( !instatiated );
	instatiated = true;
	_model_mat = get_entity(_entity_id)->get_transform()->get_model();
	content::scene::instantiate(_model, _entity_id);
}

void instanced_geometry::destroy()
{	
	assert(instatiated);
	instatiated = false;
	content::scene::remove_instance(_model, _entity_id);
}

void instanced_geometry::update()
{
	if ( !instatiated )
		return;
	if (AssimpGLMHelpers::IsMat4Same(get_entity(_entity_id)->get_transform()->get_model(),_model_mat ))
		return;
	destroy();
	instantiate();
}

instanced_geometry_id create_instanced_geometry(entity::entity_id entity, const std::string& model_name)
{
	unsigned int index = components.emplace_tombstone(entity, model_name);
	if ( index >= generations.size() )
		generations.emplace_back(0);
	
	instanced_geometry_id id = id::set_generation(index, generations[index]);
	components[index].instantiate();
	return id;
}

void remove_instanced_geometry(instanced_geometry_id id)
{
	assert(id::generation(id) == generations[id::index(id)]);
	components[id::index(id)].destroy();
	components.erase(components.internal_begin() + id::index(id));
	++generations[id::index(id)];
}

instanced_geometry* get_instanced_geometry(instanced_geometry_id id)
{
	if ( !is_valid(id) )
		return nullptr;
	return &components[id::index(id)];
}

void update_models()
{
	for ( auto& c : components )
		c.update();
}

bool is_valid(instanced_geometry_id id)
{
	if ( id == id::invalid_id )
		return false;
	return id::generation(id) == generations[id::index(id)];
}

}
