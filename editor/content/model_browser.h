#pragma once 
#include <string>
#include "utility/helpers/string_browser.h"

namespace editor::model_browser 
{

class model_browser : public string_browser::browser
{
public:
	void update();
};
}
