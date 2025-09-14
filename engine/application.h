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

private:
	std::string _texture;
	std::string _texture_specular;
	std::string _texture_fbo = "*fbo_texture";


	programs::program_id prog;

	unsigned int _FBO;
	unsigned int _RBO;
	GLFWwindow* _window;

	bool _finished = false;
	
	std::string _scene;
	std::string _scene2;
	utl::vector<ecs::entity::entity_id> entt;

};
