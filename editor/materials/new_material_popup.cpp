#include "new_material_popup.h"
#include "imgui.h"

namespace editor::materials::popup
{

namespace {	
	

	bool initialized = false;
	std::string material_name = "";

	void finalize()
	{
		initialized = false;
	}
}

void initialize()
{
	initialized = true;
	material_name = "";
}

void open()
{
	ImGui::SetNextWindowSize(ImVec2(256, 128));
	ImGui::OpenPopup("add material");
}

void update()
{
	if ( !initialized )
		return;

	if ( ImGui::BeginPopupModal("add material") )
	{
		char buf[255];
		strcpy(buf, material_name.c_str());
		ImGui::InputText("material name", buf, 255);
		material_name = buf;

		if( ImGui::Button("add material") )
		{
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

bool finsihed()
{
	return !initialized;
}

}
