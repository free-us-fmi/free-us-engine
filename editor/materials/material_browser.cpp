#include "material_browser.h"
#include "engine/utility/vector.h"
#include "engine/managers/MaterialManager.h"

namespace editor::material_browser
{

void browser::update()
{
	utl::vector<std::string> material_names;
	std::unordered_map<std::string, materials::material_id>& materials = materials::get_materials();
	
	for ( auto& material : materials )
		material_names.emplace_back(material.first);

	string_browser::browser::update(material_names);

}

}
