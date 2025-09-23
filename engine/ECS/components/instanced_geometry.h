#pragma once 
#include "core/GLCommon.h"
#include "core/common.h"
#include <string>

namespace ecs::components::instanced_geometry 
{

class instanced_geometry 
{
public:
	instanced_geometry(entity::entity_id entity, const std::string& model);
	void instantiate();
	void destroy();
	void update();

	std::string get_model_name() const { return  _model;}
private:
	glm::mat4 _model_mat;
	bool instatiated = false;
	unsigned int _instance_id;
	std::string _model;
	entity::entity_id _entity_id;
};

instanced_geometry_id create_instanced_geometry(entity::entity_id entity_id, const std::string& model_name);
void remove_instanced_geometry(instanced_geometry_id id);
instanced_geometry* get_instanced_geometry(instanced_geometry_id id);

void update_models();
bool is_valid(instanced_geometry_id id);
}
