#include "model_view.h"
#include "editor/files/file_popup.h"
#include "imgui.h"
#include "assets/AssetsPath.h"
#include "editor/content/model_browser.h"
#include "editor/utility/helpers/progress_bar/progess_popup.h"
#include "content/scene.h"
#include <filesystem>
#include <thread>
#include <mutex>

namespace editor::model 
{

namespace {
	model_browser::model_browser browser;	
}

void update()
{
	static bool popup = false;
	static bool loading = false;

	ImGui::SetNextWindowSize(ImVec2(512, 1024), ImGuiCond_FirstUseEver);
	ImGui::Begin("models");
	if ( ImGui::Button("add model") )
	{
		popup = true;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);	
		file_browser::popup::open();
	}
	
	if ( popup )
		file_browser::popup::update();

	std::string last_path = file_browser::popup::get_selected_file_path();
	if ( popup && file_browser::popup::finished() )
	{
		if ( last_path != "" )
		{
			auto t = std::thread(::content::scene::create_scene, std::filesystem::path(last_path).filename().string(), last_path, false);
			t.detach();
		}
		popup = false;
	}

	if ( !loading && content::scene::creation_started() )
	{
		loading = true;
		helpers::progress::popup::initialize("loading model");
		helpers::progress::popup::open();
	}

	if ( loading )
	{
		if ( content::scene::creation_started() )
			helpers::progress::popup::update();
		else {
			loading = false;
			helpers::progress::popup::finalize();
		}

	}

	browser.update();

	ImGui::End();
}

}
