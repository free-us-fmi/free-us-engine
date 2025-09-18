#pragma once 
#include <string>

namespace editor::file_browser::popup
{
	void initialize(const std::string& root_path, bool allow_root = false);
	void update();
	void open();
	bool finished();
	std::string get_selected_file_path();
}
