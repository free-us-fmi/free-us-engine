#pragma once
#include "core/GLCommon.h"
#include "core/common.h"
#include "shader.h"
#include <unordered_map>
#include <string>
#include "utility/vector.h"

namespace programs 
{

class program 
{
public:
	enum ShaderType
	{
		VERTEX = 0,
		FRAGMENT,
		GEOMETRY
	};

	program();
	void AddShader(ShaderType type, const std::string& path);
	void Link();
	void Bind();
	void SetDepthTest(GLenum test) { _depth_test = test; };
	unsigned int  GetId() const { return _id; }

private:
	int GetUniformLocation(const std::string& uniform_name);

private:
	GLenum _depth_test = GL_LESS;	
	bool _linked = false;
	unsigned int _id = 0;
	std::unordered_map<std::string, int>_uniform_cache;
	utl::vector<shader> _shaders;
public:
	void SetUniform1f(const std::string& uniform_name, float _1) { glUniform1f( GetUniformLocation(uniform_name), _1); }
	void SetUniform2f(const std::string& uniform_name, float _1, float _2) { glUniform2f(GetUniformLocation(uniform_name), _1, _2); }
	void SetUniform3f(const std::string& uniform_name, float _1, float _2, float _3) {  glUniform3f(GetUniformLocation(uniform_name),_1, _2, _3); }
	void SetUniform4f(const std::string& uniform_name, float _1, float _2, float _3, float _4) { glUniform4f(GetUniformLocation(uniform_name), _1, _2, _3, _4); }

	void SetUniform1i(const std::string& uniform_name, int _1){ glUniform1i(GetUniformLocation(uniform_name), _1); }
	void SetUniform2i(const std::string& uniform_name, int _1, int _2){ glUniform2i(GetUniformLocation(uniform_name), _1, _2); }
	void SetUniform3i(const std::string& uniform_name, int _1, int _2, int _3){ glUniform3i(GetUniformLocation(uniform_name), _1, _2, _3); }
	void SetUniform4i(const std::string& uniform_name, int _1, int _2, int _3, int _4){ glUniform4i(GetUniformLocation(uniform_name), _1, _2, _3, _4); }


	void SetUniform1ui(const std::string& uniform_name, unsigned int _1) { glUniform1ui(GetUniformLocation(uniform_name), _1); }
	void SetUniform2ui(const std::string& uniform_name, unsigned int _1, unsigned int _2) { glUniform2ui(GetUniformLocation(uniform_name), _1, _2); }
	void SetUniform3ui(const std::string& uniform_name, unsigned int _1, unsigned int _2, unsigned int _3) { glUniform3ui(GetUniformLocation(uniform_name), _1, _2, _3); }
	void SetUniform4ui(const std::string& uniform_name, unsigned int _1, unsigned int _2, unsigned int _3, unsigned int _4) { glUniform4ui(GetUniformLocation(uniform_name), _1, _2, _3, _4); }

	void SetUniform1fv(const std::string& uniform_name, const float* adr, int count = 1) { glUniform1fv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform2fv(const std::string& uniform_name, const float* adr, int count = 1) { glUniform2fv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform3fv(const std::string& uniform_name, const float* adr, int count = 1) { glUniform3fv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform4fv(const std::string& uniform_name, const float* adr, int count = 1) { glUniform4fv(GetUniformLocation(uniform_name), count, adr); }
	
	void SetUniform1iv(const std::string& uniform_name, const int* adr, int count = 1) { glUniform1iv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform2iv(const std::string& uniform_name, const int* adr, int count = 1) { glUniform1iv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform3iv(const std::string& uniform_name, const int* adr, int count = 1) { glUniform1iv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform4iv(const std::string& uniform_name, const int* adr, int count = 1) { glUniform1iv(GetUniformLocation(uniform_name), count, adr); }
	
	void SetUniform1uiv(const std::string& uniform_name, const unsigned int* adr, int count = 1) { glUniform1uiv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform2uiv(const std::string& uniform_name, const unsigned int* adr, int count = 1) { glUniform1uiv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform3uiv(const std::string& uniform_name, const unsigned int* adr, int count = 1) { glUniform1uiv(GetUniformLocation(uniform_name), count, adr); }
	void SetUniform4uiv(const std::string& uniform_name, const unsigned int* adr, int count = 1) { glUniform1uiv(GetUniformLocation(uniform_name), count, adr); }

	void SetUniformMatrix2fv(const std::string& uniform_name, bool transposed, const float* adr, int count = 1) {  glUniformMatrix2fv(GetUniformLocation(uniform_name), count, transposed? GL_TRUE : GL_FALSE, adr); }
	void SetUniformMatrix3fv(const std::string& uniform_name, bool transposed, const float* adr, int count = 1) {  glUniformMatrix3fv(GetUniformLocation(uniform_name), count, transposed? GL_TRUE : GL_FALSE, adr); }
	void SetUniformMatrix4fv(const std::string& uniform_name, bool transposed, const float* adr, int count = 1) {  glUniformMatrix4fv(GetUniformLocation(uniform_name), count, transposed? GL_TRUE : GL_FALSE, adr); } 
};

}
