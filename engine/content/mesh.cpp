#include "mesh.h"
#include "core/GLCommon.h"
#include "managers/TextureManager.h"
#include "thread/main_thread_dispatcher.h"
#include <mutex>
namespace content::mesh 
{

namespace {
	utl::vector<mesh, false> meshes;
}

void mesh::draw(programs::program* prog, glm::mat4 model)
{
	glBindVertexArray(_VAO);

	glm::mat4 normal_model = glm::transpose(glm::inverse(model));

	prog->SetUniformMatrix4fv("model", false, glm::value_ptr(model));
	prog->SetUniformMatrix4fv("normal_model", false, glm::value_ptr(normal_model));

	std::string _texture_specular = "";
	std::string _texture = "";

	textures::unbind_all();

	if ( _material._specular_map.size() )
	{
		std::string _texture_specular = _material._specular_map[0];
		unsigned int tex_specular_slot = textures::bind_texture(_texture_specular);
		prog->SetUniform1i("material.specular", tex_specular_slot);
	}
	if ( _material._textures_map.size())
	{
		_texture = _material._textures_map[0];
		unsigned int tex_slot = textures::bind_texture(_texture);
		prog->SetUniform1i("material.ambient", tex_slot);
	}

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, NULL);
}

mesh* get_mesh(unsigned int id)
{
	assert(id < meshes.size());
	assert(!meshes.is_tombstone(meshes.begin() + id));

	return &meshes[id];
}

unsigned int add_mesh(mesh& m)
{
	auto gl_call = [&](){
		glGenVertexArrays(1, &m._VAO);

		glGenBuffers(1, &m._VBO);
		glGenBuffers(1, &m._EBO);
		
		glBindVertexArray(m._VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m._VBO);
		glBufferData(GL_ARRAY_BUFFER, m._vertices.size() * sizeof(vertex), m._vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m._EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m._indices.size() * sizeof(u32), m._indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 *  sizeof(float), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
		glEnableVertexAttribArray(2);
	};

	std::mutex wait_for_main; 
	thread::main_thread::add_event(gl_call, wait_for_main, std::this_thread::get_id());
	
	std::lock_guard<std::mutex> lg(wait_for_main);

	unsigned int id = meshes.emplace_tombstone(m);
	return id;
}

void remove_mesh(unsigned int id)
{
	assert( id < meshes.size() );
	mesh& m = meshes[id];
	glDeleteBuffers(1, &m._EBO);
	glDeleteBuffers(1, &m._VBO);
	meshes.erase(meshes.begin() + id);
}

}
