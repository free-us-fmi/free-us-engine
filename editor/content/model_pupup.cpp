#include "model_popup.h"
#include "core/common.h"
#include "imgui.h"
#include "model_browser.h"
#include "engine/managers/ProgramManager.h"
#include "engine/utility/vector.h"
#include <unordered_set>
namespace editor::model_browser::popup
{

namespace {	
	

	bool initialized = false;
	std::string selected_model = "";
	std::unordered_set<std::string> selected_program_names;

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

	std::unordered_map<std::string, programs::program_id> programs;
	programs = programs::get_programs();

	if ( programs.find("default") != programs.end() )
	{
		selected_program_names.emplace("default");
	}
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
		ImGui::BeginChild("model popup broswer", ImVec2(ImGui::GetWindowSize().x * 0.7, ImGui::GetWindowSize().y));
		browser.update();
		ImGui::EndChild();
		ImGui::SameLine();

		std::unordered_map<std::string, programs::program_id> programs;
		programs = programs::get_programs();

		ImGui::BeginChild("model popup broswer", ImVec2(ImGui::GetWindowSize().x * 0.3, ImGui::GetWindowSize().y));
		for ( auto& program : programs )
		{
			bool select = false;
			if ( selected_program_names.find(program.first) != selected_program_names.end())
				select = true;
			if ( ImGui::Selectable(program.first.c_str(), select) )
			{
				if ( selected_program_names.find(program.first) != selected_program_names.end() )
					selected_program_names.erase(program.first);
				else 	selected_program_names.emplace(program.first);
			}
		}
		ImGui::EndChild();

		selected_model = browser.last_selected_model();
		if ( !browser.last_selected_is_empty() )
		{
			ImGui::CloseCurrentPopup();
			finalize();
		}
		ImGui::EndPopup();
	}

}

utl::vector<programs::program_id> program_result()
{
	std::unordered_map<std::string, programs::program_id> programs;
	programs = programs::get_programs();
	utl::vector<programs::program_id> result;
	for ( auto prog : selected_program_names )
		if ( programs.find(prog) != programs.end() )
			result.emplace_back(programs[prog]);
	return result;
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
