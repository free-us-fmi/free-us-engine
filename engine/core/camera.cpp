#include "camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <unordered_map>
#include "imgui/backends/imgui_impl_glfw.h"
namespace camera
{
GLFWwindow* window;
glm::vec3 position;
glm::vec2 rotation;

glm::mat4 view_matrix;

std::unordered_map<int, bool> key_map;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	if ( action == GLFW_PRESS )
		key_map[key] = true;
	else if ( action == GLFW_RELEASE )
		key_map[key] = false;
}

void change_rotation(double dx, double dy)
{

}
void MouseCursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	static float last_x = xpos, last_y = ypos;
	

	double _xpos = last_x - xpos;
	double _ypos = last_y - ypos;
	
	last_x = xpos;
	last_y = ypos;

	rotation.x += _xpos / 10;
	rotation.y += _ypos / 10;
}

float last_time; 
void Initialize(GLFWwindow* win)
{
	last_time = glfwGetTime();
	window = win;
	glfwSetKeyCallback(window, KeyCallback);
	//glfwSetCursorPosCallback(window, MouseCursorCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	position = glm::vec3(0.f, 0.f, -20.f);
	rotation = glm::vec2(180.f, 0.f);

	view_matrix = glm::mat4(1.f);
}

double camera_speed = 2;


glm::vec3 forward = glm::vec4(0.f, 0.f, -1.f, 1.f);

void Update()
{
	float new_time = glfwGetTime();
	float delta = new_time - last_time;
	
	last_time = new_time;

	glm::vec3 up = glm::vec4(0.f, 1.f, 0.f, 1.f);
	forward = glm::vec4(0.f, 0.f, -1.f, 1.f);
	glm::mat4 transform = glm::mat4(1.f);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));
	
	glm::mat3 vec_transform = glm::transpose(glm::inverse(transform));
	up = glm::normalize(vec_transform * up);
	forward = glm::normalize(vec_transform * forward);
	
	glm::vec3 difference = glm::vec3(0.f, 0.f, 0.f);

	rotation.x += (float)key_map[GLFW_KEY_LEFT] * delta * 60.f;
	rotation.x -= (float)key_map[GLFW_KEY_RIGHT] * delta * 60.f;
	rotation.y -= (float)key_map[GLFW_KEY_DOWN] * delta * 60.f;
	rotation.y += (float)key_map[GLFW_KEY_UP] * delta * 60.f;

	if ( key_map[GLFW_KEY_W] )
		difference += forward;
	if ( key_map[GLFW_KEY_S] )
		difference  -= forward;
	
	if ( key_map[GLFW_KEY_D] )
		difference -= glm::cross(up, forward);
	if ( key_map[GLFW_KEY_A] )
		difference += glm::cross(up, forward);

	if ( key_map[GLFW_KEY_Z] )
		 difference += up;
	if ( key_map[GLFW_KEY_X] )
		difference -= up;

	difference *= delta * camera_speed;

	if ( key_map[GLFW_KEY_LEFT_SHIFT] )
		difference *= 5;
	if ( key_map[GLFW_KEY_LEFT_CONTROL] )
		difference /= 5;

	position += difference;
}

glm::mat4 GetViewMatrix()
{
	view_matrix = glm::mat4(1.f);
	view_matrix = glm::translate(view_matrix, position);
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.x), glm::vec3(0.f, 1.f, 0.f));
	view_matrix = glm::rotate(view_matrix, glm::radians(rotation.y), glm::vec3(1.f, 0.f, 0.f));

	view_matrix = glm::inverse(view_matrix);

	return view_matrix;
}

glm::vec3 GetCameraPos()
{
	return position;
}

glm::vec3 GetForward()
{	
	return forward;
}

}
