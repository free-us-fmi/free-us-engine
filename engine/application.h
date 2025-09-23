#pragma once
#include "shaders/program.h"
#include "core/GLCommon.h"
#include "core/common.h"
#include <memory>
#include "ECS/entity.h"
#include "content/scene.h"

struct GLFWwindow;

class application
{
public:
	bool Initialize();
	void Run();
	void Finalize();

	bool IsFinished() const { return _finished; }
	
	void set_fbo(unsigned int fbo) { _FBO = fbo;}
	GLFWwindow* get_window() const { return _window; }
	programs::program_id get_program() const { return prog; }
	programs::program_id get_instanced_program() const { return instanced_prog; }
private:
	std::string _texture;
	std::string _texture_specular;
	std::string _texture_fbo = "*fbo_texture";


	programs::program_id prog;
	programs::program_id instanced_prog;

	unsigned int _FBO;
	GLFWwindow* _window;

	bool _finished = false;
	
	std::string _scene;
	std::string _scene2;
	utl::vector<ecs::entity::entity_id> entt;

};
