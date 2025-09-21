#include "application.h"
#include "core/GLCommon.h"

#include "ECS/ecs.h"
#include "managers/ProgramManager.h"
#include "managers/TextureManager.h"

#include "shaders/src/ShadersPath.h"
#include "assets/AssetsPath.h"

#include "core/camera.h"

#include "glm/gtc/type_ptr.hpp"
#include "content/scene.h"
#include "thread/main_thread_dispatcher.h"
#include "utility/util.h"

void error_callback(int error, const char* description)
{	
	std::cout << "Error: " << description <<std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}


bool application::Initialize()
{
	thread::main_thread::initialize();

	if ( ! glfwInit() )
	{
		std::cout << "GLFW initialization failed!" << std::endl;
		return false;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(1600, 900, "fereastra", NULL, NULL);

	if ( ! _window )
	{
		std::cout << "Failed to create window!" << std::endl;
		return false;
	}
	glfwSetKeyCallback(_window, key_callback);
	glfwMakeContextCurrent(_window);

	glfwSwapInterval(0);
	int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	

	
	instanced_prog = programs::AddProgram();
	 prog = programs::AddProgram();

	programs::program* _program = programs::GetProgram(prog);
	programs::program* _instanced_program = programs::GetProgram(instanced_prog);

        _program->AddShader(programs::program::VERTEX,
                            shaders::GetShadersPath() + "default.vs");
        _program->AddShader(programs::program::FRAGMENT,
                            shaders::GetShadersPath() + "default.fs");
        _program->Link();

        _instanced_program->AddShader(programs::program::VERTEX,
                            shaders::GetShadersPath() + "instanced.vs");
        _instanced_program->AddShader(programs::program::FRAGMENT,
                            shaders::GetShadersPath() + "default.fs");
        _instanced_program->Link();

	_scene = assets::GetAssetsPath() + "resources/objects/rock/rock.obj";
	_scene2 = assets::GetAssetsPath() + "resources/objects/cro/cro.glb";

	content::scene::create_scene("croissant", _scene);	
	
	glEnable(GL_DEPTH_TEST);

	for ( unsigned int i = 0; i < 40000; ++i )
	{
		entt.emplace_back(ecs::create_entity("croissant" + std::to_string(i)));
		auto _entity = ecs::get_entity(entt[i]);
		_entity->get_transform()->set_position(glm::vec3((i % 200) * 5, 0.f, (i / 200) * 5));
		_entity->get_transform()->set_rotation(glm::vec3(-90.f, 0.f, 180.f));
      		content::scene::instantiate("croissant", _entity->get_transform_id());
	}

	_program->SetUniform3fv("dirLight.direction", glm::value_ptr(glm::vec3(-0.5f, -1.f, 0.3f)));
	_program->SetUniform3fv("dirLight.ambient", glm::value_ptr(glm::vec3(0.23f, 0.23f, 0.23f)));
	_program->SetUniform3fv("dirLight.diffuse", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	_program->SetUniform3fv("dirLight.specular", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));

	_instanced_program->SetUniform3fv("dirLight.direction", glm::value_ptr(glm::vec3(-0.5f, -1.f, 0.3f)));
	_instanced_program->SetUniform3fv("dirLight.ambient", glm::value_ptr(glm::vec3(0.23f, 0.23f, 0.23f)));
	_instanced_program->SetUniform3fv("dirLight.diffuse", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));
	_instanced_program->SetUniform3fv("dirLight.specular", glm::value_ptr(glm::vec3(1.f, 1.f, 1.f)));


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	return true;
}
bool first_time = true;
void application::Run()
{
	if ( first_time )
	{
		first_time = false;
		camera::Initialize(_window);
	}
	
	thread::main_thread::update();
	programs::program* _program = programs::GetProgram(prog);
	programs::program* _instanced_program = programs::GetProgram(instanced_prog);
	if ( glfwWindowShouldClose(_window))
		_finished = true;
	

	_program->Bind();

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float )1920 / (float)1080, 0.1f, 1000.0f);

	glm::mat4 view = glm::mat4(1.f);
	view = camera::GetViewMatrix();

	_instanced_program->SetUniformMatrix4fv("projection_view", false, glm::value_ptr(projection * view));
	_instanced_program->SetUniform1f("material.diffuse", 1.f);
	_instanced_program->SetUniform1i("material.shininess", 1024);
	_instanced_program->SetUniform3fv("eyePos", glm::value_ptr(camera::GetCameraPos()));
	

	_program->SetUniformMatrix4fv("projection_view", false, glm::value_ptr(projection * view));
	_program->SetUniform1f("material.diffuse", 1.f);
	_program->SetUniform1i("material.shininess", 1024);
	_program->SetUniform3fv("eyePos", glm::value_ptr(camera::GetCameraPos()));
	
	camera::Update();

	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	ecs::update();

	content::mesh::render_instanced(_instanced_program);

	glfwSwapBuffers(_window);
	glfwPollEvents();


}

void application::Finalize()
{ 
	programs::program* _program = programs::GetProgram(prog);
	glDeleteProgram(_program->GetId());
	glfwTerminate();
}

