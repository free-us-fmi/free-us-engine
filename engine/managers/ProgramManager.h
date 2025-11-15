#pragma once 
#include "core/common.h"
#include <unordered_map>
#include <filesystem>

namespace programs 
{
	class program;
	
	program_id AddProgram(const std::string& name);
	program_id AddProgram(const std::string& name, const std::filesystem::path& vertex, const std::filesystem::path& fragment);
	program_id AddProgram(const std::string& name, const std::filesystem::path& vertex, const std::filesystem::path& fragment, const std::filesystem::path& geometry);

	program* GetProgram(program_id id);	
	program* GetProgram(const std::string& name);	
	std::unordered_map<std::string, program_id>& get_programs();
	program_id GetProgramId(const std::string& name);

	void unload();

}
