#pragma once 
#include "core/common.h"
#include <unordered_map>

namespace programs 
{
	class program;
	
	program_id AddProgram(const std::string& name);
	program_id AddProgram(const std::string& name, const std::string& vertex, const std::string& fragment);
	program_id AddProgram(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry);

	program* GetProgram(program_id id);	
	program* GetProgram(const std::string& name);	
	std::unordered_map<std::string, program_id>& get_programs();
}
