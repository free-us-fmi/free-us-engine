#include "material_popup.h"
#include "material_browser.h"
#include "imgui.h"

namespace editor::material_browser::popup 
{

namespace {	
	bool initialized = false;
	browser browser;
	bool was_cancelled = false;

	void finalize()
	{
		initialized = false;
	}
}

void initialize_and_open()
{
	initialized = true;
	browser.initialize();
	was_cancelled = false;

	ImGui::SetNextWindowSize(ImVec2(800, 512));
	ImGui::OpenPopup("Select model");
}

std::string update()
{
	std::string result = "";
	if ( !initialized )
		return result;

	if(ImGui::BeginPopupModal("Select model") )
	{
		browser.update();
		result = browser.last_selected_item();

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 40);
		if (ImGui::Button("Close", ImVec2(-1, 25)))
		{
			was_cancelled = true;
			result = "";
			ImGui::CloseCurrentPopup();
			finalize();
		}

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
