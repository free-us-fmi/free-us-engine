#include "transform.h"
#include "glm/gtc/matrix_transform.hpp"
#include "managers/ProgramManager.h"
#include "utility/vector.h"
#include "raymarcher/renderer.h"
#include "shaders/program.h"

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
	assert(id::generation(id) == generations[id::index(id)]);
	transforms.erase(transforms.internal_begin() + id::index(id));
	++generations[id::index(id)];
}

bool is_valid(transform_id id)
{
	if ( id == id::invalid_id )
		return false;

	return id::generation(id) == generations[id::index(id)];
}

transform* get_transform(transform_id id )
{
	if ( !is_valid(id) )
		return nullptr;

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
	if ( _position_vec == position )
		return;
	_position_vec = position;
	_position = glm::translate(glm::mat4(1.f), position); 

	update_model();
}

void transform::set_rotation(const glm::vec3& rotation)
{
	if ( _rotation_vec == rotation )
		return;
	_rotation_vec = rotation;

	_rotation = glm::rotate(glm::mat4(1.f), glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	_rotation = glm::rotate(_rotation, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	_rotation = glm::rotate(_rotation, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

	update_model();
}

void transform::set_scale(const glm::vec3& scale)
{
	if ( _scale_vec == scale )
		return;

	_scale_vec = scale;

	_scale = glm::scale(glm::mat4(1.f), scale);

	update_model();
}

void transform::update_model()
{
	_model = _position * _rotation * _scale;
}

void update_raymarcher() {
	auto* program = programs::GetProgram(raymarching::get_program());
	if (!program or !program->linked())
		return;
	int component_number = 0;
	for ( auto& transform : transforms ) {
		program->SetUniform3fv("models[" + std::to_string(component_number) + "].scale", transform.get_scale());
		program->SetUniform3fv("models[" + std::to_string(component_number) + "].rotation", transform.get_rotation());
		program->SetUniform3fv("models[" + std::to_string(component_number) + "].position", transform.get_position());
		component_number++;
	}
	program->SetUniform1i("no_models", component_number);
}

}
