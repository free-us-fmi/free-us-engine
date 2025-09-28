#include "ProgramManager.h"
#include "shaders/program.h"

namespace programs 
{

namespace {
	utl::vector<program, false> programs;
	std::unordered_map<std::string, program_id> programs_map;
}

std::unordered_map<std::string, program_id>& get_programs()
{
	return programs_map;
}

program_id AddProgram(const std::string& name)
{
	assert( programs_map.find(name) == programs_map.end());
	program_id id = programs.emplace_tombstone();
	programs_map[name] = id;
	return id;
}

program_id AddProgram(const std::string& name, const std::string& vertex, const std::string& fragment)
{
	program_id id = AddProgram(name);
	program* prog = GetProgram(id);
	prog->AddShader(program::ShaderType::VERTEX, vertex);
	prog->AddShader(program::ShaderType::FRAGMENT, fragment);
	prog->Link();

	return id;
}

program_id AddProgram(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry)
{
	program_id id = AddProgram(name);
	program* prog = GetProgram(id);
	prog->AddShader(program::ShaderType::VERTEX, vertex);
	prog->AddShader(program::ShaderType::FRAGMENT, fragment);
	prog->AddShader(program::ShaderType::GEOMETRY, geometry);
	prog->Link();

	return id;
}

program* GetProgram(program_id id)
{
	assert(id < programs.size());
	return &programs[id];
}

program* GetProgram(const std::string& name)
{
	assert(programs_map.find(name) != programs_map.end());
	return GetProgram(programs_map[name]);
}

}
