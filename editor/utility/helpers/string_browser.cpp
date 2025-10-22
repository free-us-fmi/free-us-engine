#include "string_browser.h"
#include <unordered_map>
#include "utility/helpers/draw_browser.h"
#include "utility/EditorPath.h"

namespace editor::string_browser 
{

void browser::initialize()
{
	_last_selected_item = "";
}

void browser::update(utl::vector<std::string>& string_elements)
{
	utl::vector<helpers::draw_browser::browser_element> elements;

	for ( const auto& entry : string_elements )
	{	
		std::string tex = editor::GetEditorPath() + "internal_assets/file.png";
		elements.push_back({entry, tex});	
	}
	std::string selected_model = helpers::draw_browser::draw_browser(elements);

	if ( selected_model != "" )
		_last_selected_item = selected_model;
}

}
