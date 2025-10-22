#include "program.h"

namespace programs 
{

void program::solve_uniforms()
{
	for ( auto c : _uniforms_to_set )
		c.second();
	_uniforms_to_set.clear();
}

void program::SetUniform1f(const std::string& uniform_name, float _1) 
{ 
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform1f( location, _1); 
	};
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform2f(const std::string& uniform_name, float _1, float _2)
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform2f(location, _1, _2); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform3f(const std::string& uniform_name, float _1, float _2, float _3)
{  
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform3f(location,_1, _2, _3);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform4f(const std::string& uniform_name, float _1, float _2, float _3, float _4) 
{ 
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform4f(location, _1, _2, _3, _4); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform1i(const std::string& uniform_name, int _1)
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform1i(location, _1);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform2i(const std::string& uniform_name, int _1, int _2)
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform2i(location, _1, _2);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform3i(const std::string& uniform_name, int _1, int _2, int _3)
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform3i(location, _1, _2, _3);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform4i(const std::string& uniform_name, int _1, int _2, int _3, int _4)
{ 
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform4i(location, _1, _2, _3, _4); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}


void program::SetUniform1ui(const std::string& uniform_name, unsigned int _1) 
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform1ui(location, _1);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform2ui(const std::string& uniform_name, unsigned int _1, unsigned int _2)
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform2ui(location, _1, _2);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform3ui(const std::string& uniform_name, unsigned int _1, unsigned int _2, unsigned int _3)
{ 
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform3ui(location, _1, _2, _3);
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform4ui(const std::string& uniform_name, unsigned int _1, unsigned int _2, unsigned int _3, unsigned int _4) 
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform4ui(location, _1, _2, _3, _4); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform2fv(const std::string& uniform_name, glm::vec2 adr, int count) 
{ 
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform2fv(location, count, glm::value_ptr(adr)); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform3fv(const std::string& uniform_name, glm::vec3 adr, int count )
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform3fv(location, count, glm::value_ptr(adr)); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniform4fv(const std::string& uniform_name, glm::vec4 adr, int count )
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniform4fv(location, count, glm::value_ptr(adr)); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniformMatrix2fv(const std::string& uniform_name, bool transposed, glm::mat2 adr, int count) 
{  
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniformMatrix2fv(location, count, transposed? GL_TRUE : GL_FALSE, glm::value_ptr(adr));
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniformMatrix3fv(const std::string& uniform_name, bool transposed, glm::mat3 adr, int count)
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniformMatrix3fv(location, count, transposed? GL_TRUE : GL_FALSE, glm::value_ptr(adr));
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

void program::SetUniformMatrix4fv(const std::string& uniform_name, bool transposed, glm::mat4 adr, int count) 
{
	unsigned int location = GetUniformLocation(uniform_name);
	_uniforms_to_set[location] = [=](){
		glUniformMatrix4fv(location, count, transposed? GL_TRUE : GL_FALSE, glm::value_ptr(adr)); 
	};	
	if ( program::binded_program == _id )
		solve_uniforms();
}

}
