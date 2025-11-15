#include "ProgramManager.h"
#include "shaders/program.h"
#include "managers/ShaderManager.h"

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
	//programs[id].set_id(id);
	programs_map[name] = id;
	programs[id].set_name(name);
	return id;
}

program_id AddProgram(const std::string& name, const std::filesystem::path& vertex, const std::filesystem::path& fragment)
{
	program_id id = AddProgram(name);
	program* prog = GetProgram(id);

	shaders::shader_id vertex_id = shaders::add_shader(vertex, shaders::ShaderType::VERTEX);
	shaders::shader_id fragment_id = shaders::add_shader(fragment, shaders::ShaderType::FRAGMENT);

	prog->AddShader(vertex_id);
	prog->AddShader(fragment_id);
	prog->Link();

	return id;
}

program_id AddProgram(const std::string& name, const std::filesystem::path& vertex, const std::filesystem::path& fragment, const std::filesystem::path& geometry)
{
	program_id id = AddProgram(name);
	program* prog = GetProgram(id);

	shaders::shader_id vertex_id = shaders::add_shader(vertex, shaders::ShaderType::VERTEX);
	shaders::shader_id fragment_id = shaders::add_shader(fragment, shaders::ShaderType::FRAGMENT);
	shaders::shader_id geometry_id = shaders::add_shader(geometry, shaders::ShaderType::GEOMETRY);

	prog->AddShader(vertex_id);
	prog->AddShader(fragment_id);
	prog->AddShader(geometry_id);
	prog->Link();

	return id;
}

program* GetProgram(program_id id)
{
	if(id >= programs.size())
		return nullptr;
	return &programs[id];
}

program* GetProgram(const std::string& name)
{
	if (programs_map.find(name) == programs_map.end())
		return nullptr;
	return GetProgram(programs_map[name]);
}

program_id GetProgramId(const std::string& name) {
	assert(programs_map.find(name) != programs_map.end());
	return programs_map[name];
}

void unload() {
	for ( unsigned int i = 0; i < programs.size(); i++ ) {
		if ( !programs.is_tombstone(programs.internal_begin() + i) && programs[i]._editor_visible ){
			programs_map.erase(programs[i].get_name());
			programs.erase(programs.internal_begin() + i);
		}
	}
}

}
