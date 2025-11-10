#include "editor.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "scene_view.h"
#include "materials/material_view.h"

#include "ECS/components/selected_entity_view.h"
#include "content/model_view.h"

#include "ECS/entity_view.h"
#include "core/GLCommon.h"

#include "files/file_browser.h"
#include "assets/AssetsPath.h"
#include "managers/TextureManager.h"
#include "utility/EditorPath.h"
#include "assets/assets_view.h"
#include "assets/selected_asset_view.h"
#include "main_bar/main_menu.h"
#include "assets/assets.h"

namespace editor 
{

namespace {
	GLFWwindow* window;	
}


void initialize(const editor_init_data& data)
{

	window = data._window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	textures::add_texture(editor::GetEditorPath() + "internal_assets/folder.png");	
	textures::add_texture(editor::GetEditorPath() + "internal_assets/file.png");	

	assets::browser_assets::initialize();
	scene::initialize(data._scene_view_data);
}

void start_frame()
{
	ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	glfwSetCharCallback(window, ImGui_ImplGlfw_CharCallback);
	scene::clear();
}

void update()
{
	main_menu::update();
	scene::update();
	assets::update();
	entity::update();
	selected_asset::update();
}

void draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();
}

}
