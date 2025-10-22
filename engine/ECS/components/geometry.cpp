#include "geometry.h"
#include "content/scene.h"
#include "utility/vector.h"
#include "ECS/entity.h"
#include "managers/ProgramManager.h"
#include "ECS/ecs.h"
#include "core/camera.h"
#include "core/GLCommon.h"
#include "transform.h"
#include <map>

namespace ecs::components::geometry 
{

namespace {
	utl::vector<geometry, false, 1024> components;
	utl::vector<unsigned int> generations;
}

geometry::geometry(entity::entity_id entity, const std::string& model_name,const utl::vector<programs::program_id>& program_ids, bool texture_flipped)
{
	_entity_id = entity;
	_program_ids = program_ids;
	_name = model_name;
}

void geometry::draw(bool shadows, bool transparent)
{
	entity::entity* _entity = ecs::get_entity(_entity_id);
	content::scene::scene* _scene = content::scene::get_scene(_name);	
	assert( _entity->_transform != id::invalid_id);

	if ( _entity->_transform == id::invalid_id )
	{
		return;
	}
	if ( glm::length(_entity->get_transform()->get_position() - glm::vec3(0.f, 0.f, 0.f)) > 1000)
		return;

	for ( auto program_id : _program_ids )
	{
		programs::program* _program = programs::GetProgram(program_id);
		
		//if ( shadows && !_program->is_shadowed() )
		//	continue;

		_program->Bind();

		if ( _entity->_point_light != id::invalid_id )
			_program->SetUniform1i("isLight", true);
		else 
			_program->SetUniform1i("isLight", false);

		_scene->draw(_program, _entity->get_transform()->get_model());
	}
}

geometry_id create_geometry(entity::entity_id entity, const std::string& model_name, utl::vector<programs::program_id> program_ids, bool texture_flipped)
{
	unsigned int index = components.emplace_tombstone(entity, model_name, program_ids, texture_flipped);
	if ( index >= generations.size() )
		generations.emplace_back(0);
	
	geometry_id id = id::set_generation(index, generations[index]);

	return id;
}

void remove_geometry(geometry_id id)
{
	assert(id::generation(id) == generations[id::index(id)]);
	components.erase(components.internal_begin() + id::index(id));
	++generations[id::index(id)];
}

geometry* get_geometry(geometry_id id )
{
	if ( !is_valid(id) )
		return nullptr;

	return &components[id::index(id)];
}

void draw(bool shadows)
{
	std::map<float, geometry*> transparent_models;

	for ( auto& component : components )
	{
		if ( content::scene::get_scene(component.get_model_name())->transparent )
		{
			glm::vec3 camera_pos = camera::GetCameraPos();
			components::transform::transform* transform = ecs::get_entity(component.get_entity_id())->get_transform(); 
			float camera_distance = glm::distance(camera_pos, transform->get_position());
			transparent_models[camera_distance] =   &component;
		}
		else
			component.draw(shadows);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for ( std::map<float, geometry*>::reverse_iterator it = transparent_models.rbegin(); it != transparent_models.rend(); --it )
		it->second->draw(shadows);
	glDisable(GL_BLEND);
}

bool is_valid(geometry_id id)
{
	if ( id == id::invalid_id )
		return false;
	return id::generation(id) == generations[id::index(id)];
}

}
