#pragma once 
#include "core/common.h"
#include "components/transform.h"
#include "components/lights/point_light.h"
#include "components/geometry.h"
#include "components/instanced_geometry.h"
#include "components/raymarched_geometry.h"

#include "shaders/program.h"
namespace ecs::entity 
{


class entity 
{
friend class components::transform::transform;
friend class components::point_light::point_light;
friend class components::geometry::geometry;
friend class components::instanced_geometry::instanced_geometry;
friend class components::raymarched_geometry::raymarched_geometry;

public:
	entity();

	void initialize(entity_id id,const std::string& name = "unnamed entity");

	std::string get_name() const { return _name; }
	entity_id get_id() const { return _id; }

	components::transform::transform* get_transform();
	components::point_light::point_light* get_point_light();
	components::geometry::geometry* get_geometry();
	components::instanced_geometry::instanced_geometry* get_instanced_geometry();
	components::raymarched_geometry::raymarched_geometry* get_raymarched_geometry();

	components::transform::transform_id get_transform_id() const { return _transform; }
	components::geometry::geometry_id get_geometry_id() const { return _geometry; }
	components::instanced_geometry::instanced_geometry_id get_instanced_geometry_id() const { return _instanced_geometry; }
	components::point_light::point_light_id get_point_light_id() const { return _point_light; }
	components::raymarched_geometry::raymarched_geometry_id get_raymarched_geometry_id() const { return _raymarched_geometry; }

	void create_transform();

	void create_geometry(const std::string& model_name, utl::vector<programs::program_id> program_ids, bool texture_flipped = false);
	void remove_geometry();

	void create_raymarched_geometry();
	void remove_raymarched_geometry();

	void create_instanced_geometry(const std::string& model_name);
	void remove_instanced_geometry();

	void create_point_light(); 
	void remove_point_light();

	void update();

	void destroy();
private:
	std::string _name = "unnamed entity";
	entity_id _id{id::invalid_id};

	components::transform::transform_id _transform{id::invalid_id};
	components::point_light::point_light_id _point_light{id::invalid_id};
	components::geometry::geometry_id _geometry{id::invalid_id};
	components::instanced_geometry::instanced_geometry_id _instanced_geometry{id::invalid_id};
	components::raymarched_geometry::raymarched_geometry_id _raymarched_geometry{id::invalid_id};
};

}
