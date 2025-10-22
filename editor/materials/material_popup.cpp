#include "material_popup.h"
#include "material_browser.h"
#include "imgui.h"

namespace editor::material_browser::popup 
{

namespace {	
	bool initialized = false;
	browser browser;

	void finalize()
	{
		initialized = false;
	}
}

void initialize_and_open()
{
	initialized = true;
	browser.initialize();

	ImGui::SetNextWindowSize(ImVec2(512, 512));
	ImGui::OpenPopup("select model");
}

std::string update()
{
	std::string result = "";
	if ( !initialized )
		return result;

	if(ImGui::BeginPopupModal("select model") )
	{
		browser.update();
		result = browser.last_selected_item();

		if ( result != "" )
		{
			ImGui::CloseCurrentPopup();
			finalize();
		}
		ImGui::EndPopup();
	}

	return result;
}

}
