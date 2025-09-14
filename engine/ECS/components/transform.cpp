#include "transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "utility/vector.h"
#include <iostream>

namespace ecs::components::transform
{
	namespace {
		utl::vector<transform, false, 1024> transforms;
		utl::vector<unsigned int> generations;  
	}

transform_id create_transform(entity::entity_id entity)
{
	unsigned int index = transforms.emplace_tombstone(entity);
   	if ( transforms.size() > generations.size() )
		generations.emplace_back(0);

	transform_id id = id::set_generation(index, generations[index]); 	
	return id;
}

void delete_transform(transform_id id)
{
	assert( id::generation(id) < generations.size());
	assert(id::generation(id) == generations[id::index(id)]);
	transforms.erase(transforms.begin() + id::index(id));
	++generations[id::index(id)];
}

bool is_valid(transform_id id)
{
	assert(id::is_valid(id));
	return id::generation(id) == generations[id::index(id)];
}

transform* get_transform(transform_id id )
{
	assert(id::index(id) < transforms.size());
	assert(id::generation(id) == generations[id::index(id)]);

	return &transforms[id::index(id)];
}

transform::transform(entity::entity_id entity)
{
	_position_vec = glm::vec3(0.f);
	_rotation_vec = glm::vec3(0.f);
	_scale_vec = glm::vec3(1.f);

	_position = glm::mat4(1.f);
	_rotation = glm::mat4(1.f);
	_scale = glm::mat4(1.f);

	update_model();
}

void transform::set_position(const glm::vec3& position)
{
	_position_vec = position;
	_position = glm::translate(glm::mat4(1.f), position); 

	update_model();
}

void transform::set_rotation(const glm::vec3& rotation)
{
	_rotation_vec = rotation;

	_rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	_rotation = glm::rotate(_rotation, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	_rotation = glm::rotate(_rotation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

	update_model();
}

void transform::set_scale(const glm::vec3& scale)
{
	_scale_vec = scale;

	_scale = glm::scale(glm::mat4(1.f), scale);

	update_model();
}

void transform::update_model()
{
	_model = _position * _rotation * _scale;
}

}
