#include "EditorPath.h"
#include <filesystem>

namespace editor 
{
	std::string GetEditorPath()
	{
		return std::string(std::filesystem::path(__FILE__).parent_path().parent_path().string() + "/");
	}
}
