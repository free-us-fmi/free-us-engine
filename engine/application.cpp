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

#include "content/primitives/primitive_loader.h"

#include "imgui.h"
#include "materials/materials.h"
#include "raymarcher/renderer.h"

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
	glfwWindowHint(GLFW_SAMPLES, 4);

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
	

	prog = programs::AddProgram("default", shaders::GetShadersPath() + "default.vs", shaders::GetShadersPath() + "default.fs");
	ray_prog = programs::AddProgram("default_ray", shaders::GetShadersPath() + "raymarching/raymarch.vs", shaders::GetShadersPath() + "raymarching/raymarch.fs");
	instanced_prog = programs::AddProgram("instanced", shaders::GetShadersPath() + "instanced.vs", shaders::GetShadersPath() + "default.fs");

	programs::program_id sin_waves = programs::AddProgram("sin_waves",
								shaders::GetShadersPath() + "waves/klein.vs",
						       		shaders::GetShadersPath() + "default.fs"
						       		);
	programs::program_id wire_frame = programs::AddProgram("wires", 
							shaders::GetShadersPath() + "waves/klein.vs",
							shaders::GetShadersPath() + "wireframe/wireframe.fs",
							shaders::GetShadersPath() + "wireframe/wireframe.gs");

	main_programs.emplace_back(sin_waves);
	main_programs.emplace_back(instanced_prog);
	main_programs.emplace_back(prog);
	main_programs.emplace_back(wire_frame);

	ecs::components::point_light::add_lighted_program(prog);
	ecs::components::point_light::add_lighted_program(instanced_prog);
	ecs::components::point_light::add_lighted_program(sin_waves);

	int maxSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	std::cout << maxSamples;
	_scene = assets::GetAssetsPath() + "resources/objects/shrek/shrek.glb";
	_scene2 = assets::GetAssetsPath() + "resources/objects/planet/planet.obj";

	content::primitives::primitive_data p_data;
	p_data.scale = 1.f;
	p_data.divisions_x = 10;
	p_data.divisions_z = 10;
	content::primitives::create_primitive("plane", content::primitives::primitive_types::plane, p_data);
	materials::material mat;

	content::scene::create_scene("rock", _scene);	
	content::scene::create_scene("planet", _scene2);	

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth(2.5f);
	float radius = 60.0;
	float offset = 10.f;
	int amount = 1;
	for ( unsigned int i = 0; i < amount; ++i )
	{	
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller than x/z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		entt.emplace_back(ecs::create_entity("rock" + std::to_string(i)));
		auto _entity = ecs::get_entity(entt[i]);
		_entity->get_transform()->set_position(glm::vec3(x,y,z));
		float scale = (rand() % 20) / 100.0f + 0.05;
		_entity->get_transform()->set_scale(glm::vec3(scale, scale, scale));
		float rotAnglex = (rand() % 360);
		float rotAngley = (rand() % 360);
		float rotAnglez = (rand() % 360);
		_entity->get_transform()->set_rotation(glm::vec3(rotAnglex, rotAngley, rotAnglez));
		_entity->create_instanced_geometry("rock");
	}

	entt.emplace_back(ecs::create_entity("plane"));

	auto _entity = ecs::get_entity(entt[entt.size() - 1]);
	utl::vector<programs::program_id> planet_shaders;
	//planet_shaders.emplace_back(wire_frame);
	planet_shaders.emplace_back(prog);
	//planet_shaders.emplace_back(wire_frame);
	_entity->create_geometry("planet", planet_shaders);
	_entity->get_transform()->set_position(glm::vec3(0.0f, -1.f, 0.f));
	_entity->get_transform()->set_scale(glm::vec3(1.0f, 1.f, 1.f));

	for ( auto& prog : main_programs )
	{
		programs::program* program = programs::GetProgram(prog);

		program->SetUniform3fv("dirLight.direction", glm::vec3(-0.0f, -1.f, 0.1f));
		program->SetUniform3fv("dirLight.ambient", glm::vec3(0.23f, 0.23f, 0.23f));
		program->SetUniform3fv("dirLight.diffuse", glm::vec3(0.f, 0.f, 0.f));
		program->SetUniform3fv("dirLight.specular", glm::vec3(1.f, 1.f, 1.f));
	}

	shadows_fbo = data::framebuffer::AddFramebuffer(2048, 2048);
	data::framebuffer::framebuffer* sfb = data::framebuffer::GetFramebuffer(shadows_fbo);
	sfb->add_texture_2d("depth_map", GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT, false, GL_FLOAT);

	raymarching::initialize();
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);


	return true;
}
bool first_time = true;

void application::render_raster() {
	programs::program* _program = programs::GetProgram(prog);
	programs::program* _instanced_program = programs::GetProgram(instanced_prog);

	_program->Bind();

	static glm::vec3 shadow_angle = { 1.f, -1.f, 1.f };

	for ( auto& prog : main_programs )
	{
		programs::program* program = programs::GetProgram(prog);
		program->SetUniform3fv("dirLight.direction", shadow_angle);
	}

	glm::mat4 light_projection = glm::ortho(-25.f, 25.f, -25.f, 25.f, 0.1f, 25.5f);
	glm::vec3 dir_light_direction = glm::normalize(glm::vec3(shadow_angle.x, shadow_angle.y, shadow_angle.z));
	glm::vec3 dir_light_position = -11.25f * dir_light_direction;
	glm::mat4 light_view = glm::lookAt(dir_light_position, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float )1920 / (float)1080, 0.1f, 1000.0f);
	glm::mat4 view = glm::mat4(1.f);
	view = camera::GetViewMatrix();

	ecs::update();

	//DRAWING
	data::framebuffer::framebuffer* sfb = data::framebuffer::GetFramebuffer(shadows_fbo);
	sfb->bind();
	sfb->clear();
	for ( auto& prog : main_programs )
	{
		programs::program* program = programs::GetProgram(prog);

		program->SetUniformMatrix4fv("projection_view", false,light_projection * light_view);
		program->SetUniform1f("material.diffuse", 1.f);
		program->SetUniform1i("material.shininess", 1024);
		program->SetUniform1f("time", glfwGetTime());
		program->SetUniform1i("shadowed", true);
	}
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	ecs::draw();
	content::mesh::render_instanced(_instanced_program);
	//glDisable(GL_CULL_FACE);


	unsigned int fbo_slot = sfb->bind_texture("depth_map");
	for ( auto& prog : main_programs )
	{
		programs::program* program = programs::GetProgram(prog);

		program->SetUniformMatrix4fv("light_view", false,light_projection * light_view);
		program->SetUniformMatrix4fv("projection_view", false,projection * view);
		program->SetUniform3fv("eyePos", camera::GetCameraPos());
		program->SetUniform1i("shadowed", false);
		program->SetUniform1i("shadow_map", fbo_slot);
	}


	data::framebuffer::GetFramebuffer(_FBO)->bind();

	content::mesh::render_instanced(_instanced_program);
	ecs::draw();

	textures::unbind_all();
}

namespace {
	bool raymarched = true;
}

void set_raymarch(bool value) {
	raymarched = value;
}

void application::Run()
{
	if ( first_time )
	{
		first_time = false;
		camera::Initialize(_window);
	}

	camera::Update();

	thread::main_thread::update();

	if ( glfwWindowShouldClose(_window))
		_finished = true;

	if ( !raymarched )
		render_raster();
	else
		raymarching::render();

	glfwSwapBuffers(_window);
	glfwPollEvents();
}

void application::Finalize()
{ 
	programs::program* _program = programs::GetProgram(prog);
	glDeleteProgram(_program->GetId());
	glfwTerminate();
}

