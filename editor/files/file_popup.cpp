#include "file_popup.h"
#include "file_browser.h"
#include "core/common.h"
#include "imgui.h"
#include <filesystem>

namespace editor::file_browser::popup
{

namespace {	
	file_browser browser;	
	bool initialized = false;


	void finalize()
	{
		initialized = false;
	}
}

void initialize(const std::string& root_path, bool allow_root)
{
	if ( initialized )
		spdlog::error("Warning! filebrowser popup can only be used once at a time!");

	editor_init_data browser_init_data;
	browser_init_data._name = "select file";
	browser_init_data._dir_path = root_path;
	browser.initialize(browser_init_data, allow_root);
	
	initialized = true;
	

}

void open() 
{
	ImGui::SetNextWindowSize(ImVec2(512, 512));
	ImGui::OpenPopup("select file");
}

void update()
{
	if ( !initialized )
		return;

	if(ImGui::BeginPopupModal("select file") )
	{
		browser.update();
		if ( !browser.last_selected_is_empty() )
		{
			ImGui::CloseCurrentPopup();
			finalize();
		}
		ImGui::EndPopup();
	}
}

std::string get_selected_file_path()
{
	return browser.last_selected_path();
}


bool finished()
{
	return initialized == false;
}


}
