#include "progess_popup.h"
#include "progress_bar.h"
#include "core/common.h"
#include "imgui.h"

namespace editor::helpers::progress::popup
{

namespace {
	progress_bar _progress_bar;
	bool initialized = false;
}

void finalize() 
{
	initialized = false;
}
	
void initialize(const std::string& process_name)
{
	if ( initialized )
	{
		return;
	}

	initialized = true;
	_progress_bar.initialize(process_name);
}

void open()
{
	ImGui::SetNextWindowSize(ImVec2(512, 64));
	ImGui::OpenPopup(_progress_bar._process_name.c_str());
}

void update()
{
	if ( ImGui::BeginPopupModal(_progress_bar._process_name.c_str()) )
	{
		if ( !initialized )
			ImGui::CloseCurrentPopup();
		_progress_bar.update();
		ImGui::EndPopup();
	}
}


bool finsihed()
{
	return !initialized;
}

}
