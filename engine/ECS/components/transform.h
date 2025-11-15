#pragma once 
#include "glm/glm.hpp" 
#include "core/common.h"

namespace ecs::entity 
{
	class entity;
}

namespace ecs::components::transform 
{
class transform
{
public:
	friend class entity::entity;
	
	explicit transform(entity::entity_id entity);

	void set_position(const glm::vec3& position);
	void set_rotation(const glm::vec3& rotation);
	void set_scale(const glm::vec3& scale);
	
	glm::vec3 get_position() const { return _position_vec; }
	glm::vec3 get_rotation() const { return _rotation_vec; }
	glm::vec3 get_scale() const { return _scale_vec; }
	

	glm::mat4 get_model() const { return _model; }
private:
	void update_model();
private:
	glm::mat4 _model;

	glm::mat4 _position;
	glm::mat4 _rotation;
	glm::mat4 _scale;

	glm::vec3 _position_vec;
	glm::vec3 _rotation_vec;
	glm::vec3 _scale_vec;
	entity::entity_id _entity_id;
};

transform_id create_transform(entity::entity_id entity);
void delete_transform(transform_id id);
transform* get_transform(transform_id id );

bool is_valid(transform_id id);

void update_raymarcher();
}
