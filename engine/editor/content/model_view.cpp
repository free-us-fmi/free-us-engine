#include "model_view.h"
#include "editor/files/file_popup.h"
#include "imgui.h"
#include "assets/AssetsPath.h"
#include "editor/content/model_browser.h"
#include "content/scene.h"
#include <filesystem>

namespace editor::model 
{

namespace {
	model_browser::model_browser browser;	
}

void update()
{
	static bool popup = false;
	ImGui::SetNextWindowSize(ImVec2(512, 1024), ImGuiCond_FirstUseEver);
	ImGui::Begin("Models");
	if ( ImGui::Button("add model") )
	{
		popup = true;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);	
	}
	
	if ( popup )
		file_browser::popup::update();

	std::string last_path = file_browser::popup::get_selected_file_path();
	if ( popup && last_path != "")
	{
		//TODO make a name request popup
		std::cout << "creating scene with path: " << last_path << " and name " << std::filesystem::path(last_path).filename().string() << std::endl;
		content::scene::create_scene(std::filesystem::path(last_path).filename().string(), last_path);
		popup = false;
	}

	browser.update();

	ImGui::End();
}

}
