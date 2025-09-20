#include "model.h"

namespace content::model 
{
namespace {
	utl::vector<model, false> models;
}

void model::draw(programs::program* prog, glm::mat4 global)
{
	for ( auto c : _meshes )
	{
		mesh::mesh* _mesh = mesh::get_mesh(c);
		_mesh->draw(prog, global * _local_model);
	}
}

model* get_model(unsigned int id)
{
	assert(id < models.size());
	assert(!models.is_tombstone(models.begin() + id));

	return &models[id];
}

unsigned int add_model(const model& m)
{
	return models.emplace_tombstone(m);
}

void remove_model(unsigned int id)
{
	assert( id < models.size() );
	models.erase(models.begin() + id);
}


void model::instantiate(unsigned int transform_id)
{
	for ( auto c : _meshes )
	{
		mesh::mesh* _mesh = mesh::get_mesh(c);
		_mesh->instantiate(transform_id);
	}
}

}
