#include "new_material_popup.h"
#include "imgui.h"

namespace editor::materials::popup
{

namespace {	
	bool initialized = false;
	std::string material_name;
	bool was_cancelled = false;
	bool show_error_popup = false;

	void finalize()
	{
		initialized = false;
		show_error_popup = false;
	}
}

void initialize()
{
	initialized = true;
	material_name = "";
	was_cancelled = false;
	show_error_popup = false;
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
		ImGui::InputTextWithHint("##material_input", "Material name", buf, 255);
		material_name = buf;

		// ERROR POPUP HANDLING
		if (show_error_popup)
		{
			ImGui::OpenPopup("Material Error");
		}

		if (ImGui::BeginPopupModal("Material Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Material name cannot be empty!");
			ImGui::Spacing();

			if (ImGui::Button("OK", ImVec2(80, 25)))
			{
				show_error_popup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		float content_height = ImGui::GetContentRegionAvail().y - 30;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + content_height);

		float button_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2;

		if( ImGui::Button("Add material", ImVec2(button_width, 25)) )
		{
			if (!material_name.empty())
			{
				finalize();
				ImGui::CloseCurrentPopup();
			}
			else
			{
				show_error_popup = true;
			}
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
	return !initialized && !was_cancelled;
}

}