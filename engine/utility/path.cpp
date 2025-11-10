#include "path.h"
#include <filesystem>
#include <algorithm>
#include <regex>
#include <string>

namespace utl{ 
void normalize_path(std::filesystem::path& path)
{
	std::string path_str = path.string();
	std::replace(path_str.begin(), path_str.end(),'\\', '/');
	std::regex r("//");
	path_str = std::regex_replace(path_str, r, "/");
	path = std::filesystem::canonical(path_str);
}

void remove_trailing_separator(std::filesystem::path& path)
{
	if ( path.string().ends_with('/') || path.string().ends_with('\\'))
		path = path.parent_path();
}

}
