#include "mesh.h"
#include "core/GLCommon.h"
#include "managers/TextureManager.h"
#include "thread/main_thread_dispatcher.h"
#include "ECS/components/transform.h"
#include <atomic>
#include <mutex>
#include "ECS/ecs.h"
#include "managers/MaterialManager.h"
namespace content::mesh 
{

namespace {
	utl::vector<mesh, false> meshes;
	std::mutex _mutex;
}

void mesh::draw(programs::program* prog,const glm::mat4& model, bool transparent) const
{
	glBindVertexArray(_VAO);

	glm::mat4 normal_model = glm::transpose(glm::inverse(model));

	prog->SetUniformMatrix4fv("model", false, model);
	prog->SetUniformMatrix4fv("normal_model", false, normal_model);

	materials::material* mat = nullptr;
	if ( _material != id::invalid_id )
		mat = materials::GetMaterial(_material);

	if ( mat ) {
		mat->bind_uniforms(materials::material::texture_type::diffuse, prog);
		mat->bind_uniforms(materials::material::texture_type::specular, prog);
	}

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL);

	if ( mat ) {
		mat->unbind_uniforms(materials::material::texture_type::diffuse);
		mat->unbind_uniforms(materials::material::texture_type::specular);
	}

}

mesh* get_mesh(unsigned int id)
{
	std::lock_guard<std::mutex> lg(_mutex);
	assert(id < meshes.size());
	assert(!meshes.is_tombstone(meshes.internal_begin() + id));

	return &meshes[id];
}

unsigned int add_mesh(mesh& m)
{
	void* VBO_data, *EBO_data;
	auto gl_call = [&](){
		glGenVertexArrays(1, &m._VAO);

		glGenBuffers(1, &m._transform_buffer);
		glGenBuffers(1, &m._VBO);
		glGenBuffers(1, &m._EBO);
		
		glBindVertexArray(m._VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m._VBO);
		glBufferStorage(GL_ARRAY_BUFFER, m._vertices.size() * sizeof(vertex), 0, GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT);
		VBO_data = glMapBufferRange(GL_ARRAY_BUFFER, 0, m._vertices.size() * sizeof(vertex), GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m._EBO);
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, m._indices.size() * sizeof(unsigned int), 0, GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT);
		EBO_data = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, m._indices.size() * sizeof(unsigned int), GL_MAP_WRITE_BIT | GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, m._transform_buffer);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float) * 4));
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);

		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float)  * 8));
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);

		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(sizeof(float) * 12));
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);
	};

	thread::main_thread::add_event_and_wait(gl_call);
	memcpy(VBO_data, m._vertices.data(), m._vertices.size() * sizeof(vertex));
	memcpy(EBO_data, m._indices.data(), m._indices.size() * sizeof(unsigned int));

	auto unmap_buffers = [&](){
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	};

	thread::main_thread::add_event_and_wait(unmap_buffers);

	std::lock_guard<std::mutex> lg(_mutex);
	unsigned int id = meshes.emplace_tombstone(m);
	return id;
}

void remove_mesh(unsigned int id)
{
	std::lock_guard<std::mutex> lg(_mutex);
	assert( id < meshes.size() );
	mesh& m = meshes[id];
	glDeleteBuffers(1, &m._EBO);
	glDeleteBuffers(1, &m._VBO);
	glDeleteVertexArrays(1, &m._VAO);
	meshes.erase(meshes.internal_begin() + id);
}

void mesh::draw_instanced(programs::program* prog) const
{
	if ( _transforms.empty() )
		return;
	
	glBindVertexArray(_VAO);
	prog->Bind();
	prog->SetUniform1i("isLight", false);
	materials::material* mat = nullptr;
	if ( _material != id::invalid_id )
		mat = materials::GetMaterial(_material); 

	if ( mat ) {
		mat->bind_uniforms(materials::material::texture_type::diffuse, prog);
		mat->bind_uniforms(materials::material::texture_type::specular, prog);
	}

	glDrawElementsInstanced(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL, _transforms.size());

	if ( mat ) {
		mat->unbind_uniforms(materials::material::texture_type::diffuse);
		mat->unbind_uniforms(materials::material::texture_type::specular);
	}

}


void mesh::instantiate(ecs::entity::entity_id entity_id, const glm::mat4& _local_model)
{
	assert (!entity_to_transform.contains(entity_id));
	if ( entity_to_transform.contains(entity_id) )
		return;

	ecs::entity::entity* entity = ecs::get_entity(entity_id);
	ecs::components::transform::transform_id transform_id = entity->get_transform_id();

	ecs::components::transform::transform* transform = ecs::components::transform::get_transform(transform_id);
	_transforms.emplace_back(transform->get_model() * _local_model);
	
	glBindVertexArray(_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, _transform_buffer);

	if ( _instance_buffer_capacity >= _transforms.capacity() )
		glBufferSubData(GL_ARRAY_BUFFER, 16 * sizeof(float) * (_transforms.size() - 1), 16 * sizeof(float), _transforms.end() - 1);
	else 
	{
		std::cout << "increased capacity to " << _transforms.capacity() << std::endl;
		_instance_buffer_capacity = _transforms.capacity();
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16 * _transforms.capacity(), nullptr, GL_DYNAMIC_DRAW);	
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16 * _transforms.size(), _transforms.data());
	}
	entity_to_transform[entity_id] = _transforms.size() - 1;
	transform_to_entity[_transforms.size() - 1] = entity_id;
}

void mesh::remove_instance(ecs::entity::entity_id entity_id)
{
	assert( entity_to_transform.contains(entity_id));
	if (!entity_to_transform.contains(entity_id) )
		return;

	ecs::components::transform::transform_id transform_id = entity_to_transform[entity_id];
	entity_to_transform.erase(entity_id);

	if ( transform_id == _transforms.size() - 1 )
	{
		_transforms.erase(_transforms.end() - 1);
		return;
	}
	ecs::entity::entity_id entity_to_be_moved = transform_to_entity[_transforms.size() - 1];
	entity_to_transform[entity_to_be_moved] = transform_id;
	transform_to_entity[transform_id] = entity_to_be_moved;

	_transforms[transform_id] = _transforms[_transforms.size() - 1];	
	_transforms.erase(_transforms.internal_end() - 1);

	glBindBuffer(GL_ARRAY_BUFFER, _transform_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(float) * 16 * transform_id, sizeof(float) * 16, &_transforms[transform_id]);

}

void render_instanced(programs::program* prog)
{
	std::lock_guard<std::mutex> lg(_mutex);
	for ( auto& mesh : meshes )
		mesh.draw_instanced(prog);
}

void mesh::set_material(unsigned int mat)
{
	_material = mat;
}

void mesh::set_material(const std::string& mat) {
	set_material(materials::GetMaterialId(mat));
}

}
