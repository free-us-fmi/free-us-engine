#include "mesh.h"
#include "core/GLCommon.h"
#include "managers/TextureManager.h"
#include "thread/main_thread_dispatcher.h"
#include "ECS/components/transform.h"
#include <atomic>
#include <mutex>
#include "ECS/ecs.h"

namespace content::mesh 
{

namespace {
	utl::vector<mesh, false> meshes;
	std::mutex _mutex;
}

void mesh::draw(programs::program* prog, glm::mat4 model)
{
	glBindVertexArray(_VAO);

	glm::mat4 normal_model = glm::transpose(glm::inverse(model));

	prog->SetUniformMatrix4fv("model", false, glm::value_ptr(model));
	prog->SetUniformMatrix4fv("normal_model", false, glm::value_ptr(normal_model));

	textures::unbind_all();

	if ( _material._specular_map.size() )
	{
		std::string _texture_specular = _material._specular_map[0];
		unsigned int tex_specular_slot = textures::bind_texture(_texture_specular);
		prog->SetUniform1i("material.specular", tex_specular_slot);
	}
	if ( _material._textures_map.size())
	{
		std::string _texture = _material._textures_map[0];
		unsigned int tex_slot = textures::bind_texture(_texture);
		prog->SetUniform1i("material.ambient", tex_slot);
	}

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL);
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
	meshes.erase(meshes.internal_begin() + id);
}

void mesh::draw_instanced(programs::program* prog)
{
	if ( !_transforms.size() )
		return;
	
	glBindVertexArray(_VAO);
	prog->Bind();
	textures::unbind_all();

	if ( _material._specular_map.size() )
	{
		std::string _texture_specular = _material._specular_map[0];
		unsigned int tex_specular_slot = textures::bind_texture(_texture_specular);
		prog->SetUniform1i("material.specular", tex_specular_slot);
	}
	if ( _material._textures_map.size())
	{
		std::string _texture = _material._textures_map[0];
		unsigned int tex_slot = textures::bind_texture(_texture);
		prog->SetUniform1i("material.ambient", tex_slot);
	}
	glDrawElementsInstanced(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL, _transforms.size());
}


void mesh::instantiate(ecs::entity::entity_id entity_id, const glm::mat4& _local_model)
{
	assert ( entity_to_transform.find(entity_id) == entity_to_transform.end() );
	if ( entity_to_transform.find(entity_id) != entity_to_transform.end() )
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
	tranform_to_entity[_transforms.size() - 1] = entity_id;
}

void mesh::remove_instance(ecs::entity::entity_id entity_id)
{
	assert( entity_to_transform.find(entity_id) != entity_to_transform.end());
	if ( entity_to_transform.find(entity_id) == entity_to_transform.end() )
		return;

	ecs::components::transform::transform_id transform_id = entity_to_transform[entity_id];
	entity_to_transform.erase(entity_id);

	if ( transform_id == _transforms.size() - 1 )
	{
		_transforms.erase(_transforms.end() - 1);
		return;
	}
	ecs::entity::entity_id entity_to_be_moved = tranform_to_entity[_transforms.size() - 1];
	entity_to_transform[entity_to_be_moved] = transform_id;
	tranform_to_entity[transform_id] = entity_to_be_moved;

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
}
