#pragma once
#include "core/GLCommon.h"
#include "core/common.h"
#include "shader.h"
#include <unordered_map>
#include <map>
#include <string>
#include "utility/vector.h"
#include <functional>
#include "glm/gtc/type_ptr.hpp"
#include <string>

namespace programs 
{

class program 
{
public:
	program();
	void AddShader(shaders::shader_id shader_id);
	void Link();
	void Bind();
	void SetDepthTest(GLenum test) { _depth_test = test; };
	unsigned int  GetId() const { return _id; }
	bool is_shadowed() const { return _shadowed; }
	bool linked() const { return _linked; }

	std::string get_name() const { return _name; }
	void set_name(const std::string& name) { _name = name; }

	void set_shadowed(bool value) { _shadowed = value; }
	void set_id(unsigned int id) { _id = id; }

	bool _editor_visible = false;
private:

	static unsigned int binded_program;

	int GetUniformLocation(const std::string& uniform_name);
	void solve_uniforms();
private:
	std::string _name;

	GLenum _depth_test = GL_LESS;	
	bool _shadowed = false;

	bool _linked = false;
	unsigned int _id = id::invalid_id;
	std::unordered_map<std::string, int>_uniform_cache;
	std::map<unsigned int, std::function<void()>> _uniforms_to_set;

	utl::vector<shaders::shader_id> _shaders;

public:
	void SetUniform1f(const std::string& uniform_name, float _1);
	void SetUniform2f(const std::string& uniform_name, float _1, float _2);
	void SetUniform3f(const std::string& uniform_name, float _1, float _2, float _3);
	void SetUniform4f(const std::string& uniform_name, float _1, float _2, float _3, float _4);

	void SetUniform1i(const std::string& uniform_name, int _1);
	void SetUniform2i(const std::string& uniform_name, int _1, int _2);
	void SetUniform3i(const std::string& uniform_name, int _1, int _2, int _3);
	void SetUniform4i(const std::string& uniform_name, int _1, int _2, int _3, int _4);

	void SetUniform1ui(const std::string& uniform_name, unsigned int _1);
	void SetUniform2ui(const std::string& uniform_name, unsigned int _1, unsigned int _2);
	void SetUniform3ui(const std::string& uniform_name, unsigned int _1, unsigned int _2, unsigned int _3);
	void SetUniform4ui(const std::string& uniform_name, unsigned int _1, unsigned int _2, unsigned int _3, unsigned int _4);

	void SetUniform2fv(const std::string& uniform_name, glm::vec2 adr, int count = 1);
	void SetUniform3fv(const std::string& uniform_name, glm::vec3 adr, int count = 1);
	void SetUniform4fv(const std::string& uniform_name, glm::vec4 adr, int count = 1);
	
	void SetUniformMatrix2fv(const std::string& uniform_name, bool transposed, glm::mat2 adr, int count = 1);
	void SetUniformMatrix3fv(const std::string& uniform_name, bool transposed, glm::mat3 adr, int count = 1);
	void SetUniformMatrix4fv(const std::string& uniform_name, bool transposed, glm::mat4 adr, int count = 1);
};

}
