#include "progress_bar.h"
#include "imgui.h"
namespace editor::helpers::progress 
{
void progress_bar::initialize(const std::string& process_name)
{
	_process_name = process_name;
}

void progress_bar::update()
{
	ImGui::ProgressBar(- ImGui::GetTime() * 0.25);

}


}
