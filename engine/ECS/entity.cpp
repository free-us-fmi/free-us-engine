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
	if ( components::transform::is_valid(_transform) )
		components::transform::delete_transform(_transform);
	if ( components::point_light::is_valid(_point_light) )
		components::point_light::delete_point_light(_point_light);
	if ( components::geometry::is_valid(_geometry))
		components::geometry::remove_geometry(_geometry);
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

void entity::create_geometry(const std::string& model_path, programs::program_id program_id, bool textrue_flipped)
{
	_geometry = components::geometry::create_geometry(_id, model_path, program_id, textrue_flipped);
}

components::geometry::geometry* entity::get_geometry()
{
	return components::geometry::get_geometry(_geometry);
}

}
