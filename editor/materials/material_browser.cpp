#include "material_browser.h"
#include "engine/utility/vector.h"
#include "engine/managers/MaterialManager.h"

namespace editor::material_browser
{

void browser::update()
{
	utl::vector<std::string> material_names;
	utl::vector<::materials::material, false>& materials = materials::get_materials();
	
	for ( auto& material : materials )
		material_names.emplace_back(material._name);

	string_browser::browser::update(material_names);
}

}
