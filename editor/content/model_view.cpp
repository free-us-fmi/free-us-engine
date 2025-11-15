#include "model_view.h"
#include "editor/files/file_popup.h"
#include "imgui.h"
#include "assets/AssetsPath.h"
#include "editor/content/model_browser.h"
#include "editor/utility/helpers/progress_bar/progess_popup.h"
#include "assets/selected_asset_view.h"
#include "content/scene.h"
#include <filesystem>
#include <thread>
#include <mutex>

namespace editor::model 
{

namespace {
	model_browser::model_browser browser;	
	std::string selected_model = "";
}

std::string get_selected_model()
{
	return selected_model;
}

void update()
{
	static bool popup = false;
	static bool loading = false;

	if ( browser.last_selected_item() != selected_model && !browser.last_selected_is_empty())
	{
		std::cout << "selected model " << browser.last_selected_item() << std::endl;
		selected_model = browser.last_selected_item();
		selected_asset::set_selected_asset_type(selected_asset::last_selected_asset_type::models);
	}

	ImGui::Spacing();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); // Transparent black background
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.2f)); // Dark gray on hover
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // Gray when pressed
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5, 0.5, 0.5, 0.5)); // White border
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f); // Border thickness

	if ( ImGui::Button("Add model", ImVec2(300, 0)) )
	{
		popup = true;
		file_browser::popup::initialize(assets::GetAssetsPath(), true);
		file_browser::popup::open();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor(4);

	ImGui::Spacing();

	float available_height = ImGui::GetContentRegionAvail().y;
	ImGui::BeginChild("ModelList", ImVec2(0, available_height), true);
	{
		ImGui::Spacing();
		browser.update();
	}
	ImGui::EndChild();
	
	if ( popup )
		file_browser::popup::update();

	std::string last_path = file_browser::popup::get_selected_file_path();
	if ( popup && file_browser::popup::finished() )
	{
		if ( last_path != "" )
		{
			auto t = std::thread(::content::scene::create_scene,
				std::filesystem::path(last_path).filename().string(), last_path, false);
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

}

}
