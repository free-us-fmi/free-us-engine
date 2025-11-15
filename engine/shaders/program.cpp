#include "program.h" 
#include "core/common.h"
#include <iostream>
#include "engine/utility/vector.h"
#include "managers/ShaderManager.h"

namespace programs {

unsigned int program::binded_program = 0;

int program::GetUniformLocation(const std::string& uniform_name) 
{
	auto map_location = _uniform_cache.find(uniform_name);
	if ( map_location != _uniform_cache.end() )
		return map_location->second;

	Bind();
	int location = glGetUniformLocation(_id, uniform_name.c_str());
	if ( location == -1 )
		std::cout << "Could not find uniform: " << uniform_name << std::endl;
	_uniform_cache[uniform_name] = location;
	return location;
}

program::program()
{
	_linked = false;
}

void program::AddShader(shaders::shader_id shader_id)
{
	assert(_linked == false);
	GLenum internal_shader_type = GL_VERTEX_SHADER;

	shaders::shader* shader = shaders::get_shader(shader_id);

	switch (shader->_shader_type) {
		case shaders::ShaderType::VERTEX:
		internal_shader_type = GL_VERTEX_SHADER;
		break;
 		case shaders::ShaderType::FRAGMENT:
		internal_shader_type = GL_FRAGMENT_SHADER;
		break;
		case shaders::ShaderType::GEOMETRY:
		internal_shader_type = GL_GEOMETRY_SHADER;
		break;
		default:
		std::cout <<  "Error!" << std::endl;
		break;
	}
	_shaders.emplace_back(shader_id);
}

void program::Link()
{
	if ( _id == id::invalid_id )
		glDeleteProgram(_id);
	_id = glCreateProgram();

	_linked = false;
	for ( auto& shader_id : _shaders ) {
		auto* shader = shaders::get_shader(shader_id);
		shader->compile();
		glAttachShader(_id, shader->GetId());
	}
	glLinkProgram(_id);

	int success;
	char infoLog[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if ( !success )
	{
		glGetProgramInfoLog(_id, 512, NULL, infoLog);
		std::cout << "Error linking program!" << std::endl;
		std::cout << infoLog << std::endl;
	}
	else _linked = true;
}

void program::Bind()
{
	assert(_linked == true);
	program::binded_program = _id;
	glUseProgram(_id);
	solve_uniforms();
	glDepthFunc(_depth_test);
}

}
