#pragma once 
#include <string> 
#include <filesystem>

#include "core/common.h"

namespace editor::file_browser 
{

struct editor_init_data
{
	std::string _name;
	std::string _dir_path;
	bool directory_mode = false;
};

class file_browser 
{
public:
	void initialize(const editor_init_data& init_data, bool allow_root = false);
	bool last_selected_is_empty() const { return _last_selected_path.empty(); }
	std::string last_selected_path() const { return _last_selected_path.string();}

	void update();
private:
	bool _directory_mode = false;
	bool allow_root = false;
	std::string _name;
	std::filesystem::path _root_path;
	std::filesystem::path _current_path;
	std::filesystem::path _last_selected_path = "";
};

}
