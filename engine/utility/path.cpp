#include "path.h"
#include <filesystem>
#include <algorithm>
#include <regex>

namespace utl{ 
void normalize_path(std::string& path)
{
	std::replace(path.begin(), path.end(),'\\', '/');
	std::regex r("//");
	path = std::regex_replace(path, r, "/");
	auto p = std::filesystem::canonical(path);
	path = p.string();
}

void remove_trailing_separator(std::filesystem::path& path)
{
	if ( path.string().ends_with('/') || path.string().ends_with('\\'))
		path = path.parent_path();
}

}
