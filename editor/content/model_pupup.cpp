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

	bool transparent = false;
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
	transparent = false;
	selected_model = "";
	browser.initialize();

	std::unordered_map<std::string, programs::program_id> programs;
	programs = programs::get_programs();

	if ( programs.find("default") != programs.end() )
	{
		selected_program_names.emplace("default");
	}
}

void open()
{
	ImGui::SetNextWindowSize(ImVec2(512, 512));
	ImGui::OpenPopup("Select model");
}

void update()
{
	if ( !initialized )
		return;

	if(ImGui::BeginPopupModal("Select model") )
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

		ImGui::Checkbox("transparent", &transparent);

		ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x - 85, ImGui::GetWindowSize().y - 35));
		if (ImGui::Button("Cancel", ImVec2(80, 25)))
		{
			selected_model = "";
			ImGui::CloseCurrentPopup();
			finalize();
		}

		selected_model = browser.last_selected_item();
		if ( !browser.last_selected_is_empty() )
		{
			ImGui::CloseCurrentPopup();
			finalize();
		}
		ImGui::EndPopup();
	}

}

bool transparent_result()
{
	return transparent;
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
