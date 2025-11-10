#include "entity.h"
#include "shaders/program.h"
namespace ecs::entity 
{

entity::entity()
{
}

void entity::initialize(entity_id id,const std::string& name)
{
	_name = name;
	_id = id;
}

void entity::create_transform()
{
	_transform = components::transform::create_transform(_id);
}

void entity::destroy()
{
	components::transform::delete_transform(_transform);
	remove_point_light();
	remove_geometry();
	remove_instanced_geometry();
}

components::transform::transform* entity::get_transform()
{
	return components::transform::get_transform(_transform);
}

void entity::create_point_light()
{
	_point_light = components::point_light::create_point_light(_id);
}

components::point_light::point_light* entity::get_point_light()
{
	return components::point_light::get_point_light(_point_light);
}

void entity::create_geometry(const std::string& model_name, utl::vector<programs::program_id> program_ids, bool texture_flipped)
{
	_geometry = components::geometry::create_geometry(_id, model_name, program_ids, texture_flipped);
}

components::geometry::geometry* entity::get_geometry()
{
	return components::geometry::get_geometry(_geometry);
}

void entity::create_instanced_geometry(const std::string& model_name)
{
	_instanced_geometry = components::instanced_geometry::create_instanced_geometry(_id, model_name);
}

components::instanced_geometry::instanced_geometry* entity::get_instanced_geometry()
{
	return components::instanced_geometry::get_instanced_geometry(_instanced_geometry);
}

void entity::remove_geometry()
{
	if ( components::geometry::is_valid(_geometry))
		components::geometry::remove_geometry(_geometry);
	_geometry = id::invalid_id;

}

void entity::remove_instanced_geometry()
{
	if ( components::instanced_geometry::is_valid(_instanced_geometry) )
		components::instanced_geometry::remove_instanced_geometry(_instanced_geometry);
	_instanced_geometry = id::invalid_id;
}

void entity::remove_point_light()
{
	if ( components::point_light::is_valid(_point_light) )
		components::point_light::delete_point_light(_point_light);
	_point_light = id::invalid_id;
}

}
