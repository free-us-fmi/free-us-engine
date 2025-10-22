#include "primitive_loader.h"
#include "content/scene.h"
namespace content::primitives 
{

namespace {
void create_plane(const primitive_data& data, mesh::mesh& m)
{
	utl::vector<vertex>& vertices = m._vertices;
	utl::vector<unsigned int>& indices = m._indices;

	unsigned int x_steps = data.divisions_x;
	unsigned int z_steps = data.divisions_z;
	
	float u_step = 1.f / (float)data.divisions_x;
	float v_step = 1.f / (float)data.divisions_z;
	
	float scale = data.scale;

	float x_step = scale / x_steps * 2;
	float z_step = scale / z_steps * 2;

	float current_x = -scale; 
	float current_u = 0.f;
	
	//vertices
	for ( unsigned int i = 0; i <= x_steps; ++i )
	{
		float current_z = -scale;
		float current_v = 1.f;

		for ( unsigned int j = 0; j <= z_steps; ++j )
		{
			vertex v;
			v.normal = { 0.f, 1.f, 0.f };
			v.uv = { current_u, current_v };
			v.position = { current_x, 0.f, current_z };
			
			vertices.emplace_back(v);

			current_v -= v_step;
			current_z += z_step;
		}

		current_x += x_step;
		current_u += u_step;
	}

	//indices 
	unsigned int index = 0;
	for ( unsigned int i = 0; i < x_steps; ++i )
	{
		for ( unsigned int j = 0; j < z_steps; ++j )
		{
			indices.emplace_back(index);
			indices.emplace_back(index + 1);
			indices.emplace_back(index + z_steps + 1);

			indices.emplace_back(index + 1);
			indices.emplace_back(index + z_steps + 1 + 1);
			indices.emplace_back(index + z_steps + 1);

			index++;
		}
		index++;	
	}
}

}

primitive_data get_default_plane_data()
{
	primitive_data _data;
	_data.scale = 1.f;
	_data.divisions_x = 100;
	_data.divisions_z = 100;

	return _data;
}

primitive_data get_default_cube_data()
{
	primitive_data _data;
	_data.scale = 1.f;
	_data.divisions_x = 1;
	_data.divisions_z = 1;

	return _data;
}


void create_primitive(const std::string& name, primitive_types type, const primitive_data& creation_data)
{
	scene::create_empty_scene(name);
	scene::scene* scene = scene::get_scene(name);
	model::model model{};
	mesh::mesh mesh{};

	model._local_model = glm::mat4(1.f);
	


	switch(type)
	{
		case primitive_types::plane:
			create_plane(creation_data, mesh);
		break;
		default:
		break;
	};
	
	unsigned int mesh_id = mesh::add_mesh(mesh);
	model._meshes.emplace_back(mesh_id);
	unsigned int model_id = model::add_model(model);
	scene->_models.emplace_back(model_id);
}

}
