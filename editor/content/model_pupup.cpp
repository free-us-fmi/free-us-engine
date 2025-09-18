#include "model_popup.h"
#include "core/common.h"
#include "imgui.h"
#include "model_browser.h"
namespace editor::model_browser::popup
{

namespace {	
	

	bool initialized = false;
	std::string selected_model = "";
	model_browser browser;

	void finalize()
	{
		initialized = false;
	}
}

void initialize()
{
	initialized = true;
	selected_model = "";
	browser.initialize();
}

void open()
{
	ImGui::SetNextWindowSize(ImVec2(512, 512));
	ImGui::OpenPopup("select model");
}

void update()
{
	if ( !initialized )
		return;

	if(ImGui::BeginPopupModal("select model") )
	{
		browser.update();
		selected_model = browser.last_selected_model();
		if ( !browser.last_selected_is_empty() )
		{
			ImGui::CloseCurrentPopup();
			finalize();
		}
		ImGui::EndPopup();
	}

}

std::string result()
{
	return selected_model;
}


bool finsihed()
{
	return !browser.last_selected_is_empty();
}


}
