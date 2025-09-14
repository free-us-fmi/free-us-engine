#pragma once 
#include "core/GLCommon.h"
#include "imgui/imgui.h"
#include "scene_view.h"
namespace editor 
{

struct editor_init_data
{
	GLFWwindow* _window;
	scene::editor_init_data _scene_view_data;
	programs::program_id program;
};

void initialize(const editor_init_data& data);
void start_frame();

void update();

void draw();

}
