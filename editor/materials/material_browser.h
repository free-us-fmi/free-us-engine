#pragma once 
#include "utility/helpers/string_browser.h"
namespace editor::material_browser 
{

class browser : public string_browser::browser
{
public:
	void update();
};

}
