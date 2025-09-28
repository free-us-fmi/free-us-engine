#pragma once 
#include "core/common.h"
#include <string>
#include "shaders/program.h"
namespace ecs::entity 
{
class entity;
}

namespace ecs::components::geometry
{

class geometry 
{
public:
	geometry(entity::entity_id entity, const std::string& model_name,const utl::vector<programs::program_id>& program_ids, bool texture_flipped = false);
	std::string get_model_name() const { return  _name;}

	void draw();
private:
	utl::vector<programs::program_id> _program_ids;
	entity::entity_id _entity_id;
	std::string _name = "";
};

geometry_id create_geometry(entity::entity_id entity, const std::string& model_path, utl::vector<programs::program_id> program_ids, bool texture_flipped = false);
void remove_geometry(geometry_id id);

geometry* get_geometry(geometry_id id);
bool is_valid(geometry_id id);

void draw();

}
