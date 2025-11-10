#include "model.h"
#include "managers/MaterialManager.h"

namespace content::model 
{
namespace {
	utl::vector<model, false> models;
}

void model::draw(programs::program* prog,const glm::mat4& global, bool transparent)
{
	for ( auto c : _meshes )
	{
		mesh::mesh* _mesh = mesh::get_mesh(c);
		_mesh->draw(prog, global * _local_model, transparent);
	}
}

model* get_model(unsigned int id)
{
	assert(id < models.size());
	assert(!models.is_tombstone(models.internal_begin() + id));

	return &models[id];
}

unsigned int add_model(const model& m)
{
	return models.emplace_tombstone(m);
}

void remove_model(unsigned int id)
{
	assert( id < models.size() );
	auto& model = models[id];
	for ( auto mesh : model._meshes )
		mesh::remove_mesh(mesh);
	models.erase(models.internal_begin() + id);
}


void model::set_material(std::string mat)
{
	for ( auto mesh_id : _meshes )
	{
		mesh::mesh* mesh = mesh::get_mesh(mesh_id);
		mesh->set_material(mat);
	}
}

void model::instantiate(ecs::entity::entity_id entity_id)
{
	for ( auto m : _meshes )
	{
		mesh::mesh* _mesh = mesh::get_mesh(m);
		_mesh->instantiate(entity_id, _local_model);
	}
}

void model::remove_instance(ecs::entity::entity_id entity_id)
{
	for ( auto m : _meshes )
	{
		mesh::mesh* _mesh = mesh::get_mesh(m);
		_mesh->remove_instance(entity_id);
	}
}

}
