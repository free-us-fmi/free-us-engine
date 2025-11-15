#include "file_browser.h"
#include "editor/editor_common.h"
#include "editor/utility/EditorPath.h"
#include "managers/TextureManager.h"
#include <algorithm>

#include "assets/AssetsPath.h"
#include "editor/utility/helpers/draw_browser.h"
#include "utility/path.h"
#include "assets/assets.h"

namespace editor::file_browser 
{

void file_browser::initialize(const editor_init_data& init_data,bool allow_root)
{
	this->allow_root = allow_root;
	_root_path = std::filesystem::absolute(init_data._dir_path);
	_name = init_data._name;
	_last_selected_path = "";
	_current_path = std::filesystem::path(_root_path);
	_directory_mode = init_data.directory_mode;
}

void file_browser::update()
{
	ImGui::Text("%s", _current_path.string().c_str());

	if ( ImGui::Button("Back") && (_root_path.compare(_current_path) < 0 || allow_root))
	{
#ifdef _WIN32
		if ( _current_path.string().ends_with('\\'))
			_current_path = _current_path.parent_path();
#else
		if (_current_path.string().ends_with('/'))
			_current_path = _current_path.parent_path();
#endif
		_current_path = _current_path.parent_path();
	}

	if ( _directory_mode && ImGui::Button("Select")) {
		_last_selected_path = _current_path.string();
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{
		_last_selected_path = "";
		ImGui::CloseCurrentPopup();
	}

	utl::vector<helpers::draw_browser::browser_element> elements;

	for ( const auto& entry : std::filesystem::directory_iterator(_current_path) )
	{	
		textures::texture_id tex = std::filesystem::is_directory(entry.path()) ?
									assets::browser_assets::get_texture_id(assets::browser_assets::asset::directory) :
									assets::browser_assets::get_texture_id(assets::browser_assets::asset::file);

		elements.push_back({entry.path().filename().string(), tex});
	}
	std::filesystem::path selected_path = helpers::draw_browser::draw_browser(elements);

	if ( std::filesystem::is_directory(_current_path / selected_path))
	{
		_current_path /= selected_path;
	}
	else _last_selected_path = _current_path / selected_path;


}

}
