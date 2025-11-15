#include "shader.h" 

#include <filesystem>
#include <fstream>
#include <sstream>
#include "core/common.h"
#include <iostream>
#include "utility/unmove.h"

namespace shaders {
	shader::shader(const std::filesystem::path& path, ShaderType shader_type, bool editor_visible)
	{
		_editor_visible = editor_visible;
		_path = path;
		_shader_type = shader_type;

		compile();
	}

	void shader::compile() {
		assert(!_path.empty());

		std::ifstream shader(_path.string());
		{
			std::stringstream buffer;
			buffer << shader.rdbuf();

			//std:: cout << "shader Shader code:\n" << buffer.str() << std::endl;

			GLenum internal_shader_type;
			switch (_shader_type) {
				case ShaderType::VERTEX:
					internal_shader_type = GL_VERTEX_SHADER;
					break;
				case ShaderType::FRAGMENT:
					internal_shader_type = GL_FRAGMENT_SHADER;
					break;
				case ShaderType::GEOMETRY:
					internal_shader_type = GL_GEOMETRY_SHADER;
					break;
			}

			_id = glCreateShader(internal_shader_type);

			glShaderSource(_id, 1, &utl::unmove(buffer.str().c_str()), NULL);
			glCompileShader(_id);
		}

		int success;
		char infoLog[512];
		glGetShaderiv(_id, GL_COMPILE_STATUS, &success);

		if ( !success )
		{
			glGetShaderInfoLog(_id, 512, NULL, infoLog);
			std::cout << "Error compiling shader: " << _path.string() << std::endl;
			std::cout << infoLog << std::endl;
		}
		shader.close();
	}

	void shader::Destroy()
	{
		if ( _id == id::invalid_id )
			return;
		glDeleteShader(_id);
	}
}