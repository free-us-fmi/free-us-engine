#include "new_material_popup.h"
#include "imgui.h"

namespace editor::materials::popup
{

namespace {	
	

	bool initialized = false;
	std::string material_name;
	bool was_cancelled = false;

	void finalize()
	{
		initialized = false;
	}
}

void initialize()
{
	initialized = true;
	material_name = "";
	was_cancelled = false;
}

void open()
{
	ImGui::SetNextWindowSize(ImVec2(256, 128));
	ImGui::OpenPopup("Add material");
}

void update()
{
	if ( !initialized )
		return;

	if ( ImGui::BeginPopupModal("Add material") )
	{
		char buf[255];
		strcpy(buf, material_name.c_str());
		ImGui::InputTextWithHint("##material_input","Material name", buf, 255);
		material_name = buf;

		float content_height = ImGui::GetContentRegionAvail().y - 30;

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + content_height);

		float button_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2;

		if( ImGui::Button("Add material", ImVec2(button_width,25)))
		{
			finalize();
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel", ImVec2(button_width, 25)))
		{
			was_cancelled = true;
			finalize();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

std::string result()
{
	return material_name;
}

bool finished()
{
	return !initialized;
}

}
