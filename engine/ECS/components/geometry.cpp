#include "geometry.h"
#include "content/scene.h"
#include "utility/vector.h"
#include "ECS/entity.h"
#include "managers/ProgramManager.h"
#include "ECS/ecs.h"
namespace ecs::components::geometry 
{

namespace {
	utl::vector<geometry, false, 1024> components;
	utl::vector<unsigned int> generations;
}

geometry::geometry(entity::entity_id entity, const std::string& model_name, programs::program_id program_id, bool texture_flipped)
{
	_entity_id = entity;
	_program_id = program_id;
	_name = model_name;
	if ( !content::scene::get_scene(model_name) ) 
		spdlog::error("Cannot create geometry component! Model with name {0} doesn't exist!", model_name);
}

void geometry::draw()
{
	entity::entity* _entity = ecs::get_entity(_entity_id);
	content::scene::scene* _scene = content::scene::get_scene(_name);	
	assert( _entity->_transform != id::invalid_id);

	if ( _entity->_transform == id::invalid_id )
	{
		spdlog::error("Entity with geometry component must also have a transform component!");
		return;
	}
	if ( glm::length(_entity->get_transform()->get_position() - glm::vec3(0.f, 0.f, 0.f)) > 1000)
		return;
	programs::program* _program = programs::GetProgram(_program_id);
	_scene->draw(_program, _entity->get_transform()->get_model());
}

geometry_id create_geometry(entity::entity_id entity, const std::string& model_name, programs::program_id program_id, bool texture_flipped)
{
	unsigned int index = components.emplace_tombstone(entity, model_name, program_id, texture_flipped);
	if ( index >= generations.size() )
		generations.emplace_back(0);
	
	geometry_id id = id::set_generation(index, generations[index]);

	return id;
}

void remove_geometry(geometry_id id)
{
	assert( id::generation(id) < generations.size());
	assert(id::generation(id) == generations[id::index(id)]);
	components.erase(components.begin() + id::index(id));
	++generations[id::index(id)];
}

geometry* get_geometry(geometry_id id )
{
	assert(id::index(id) < components.size());
	assert(id::generation(id) == generations[id::index(id)]);

	return &components[id::index(id)];
}

void draw()
{
	for ( auto& component : components )
		component.draw();
}

bool is_valid(geometry_id id)
{
	assert(id::is_valid(id));
	return id::generation(id) == generations[id::index(id)];
}

}
