#pragma once 
#include <string>
#include "engine/utility/vector.h"

namespace editor::string_browser 
{

class browser 
{
public:
	virtual void initialize();
	bool last_selected_is_empty() const { return _last_selected_item.empty(); }
	std::string last_selected_item() const { return _last_selected_item;}
	virtual void update(utl::vector<std::string>& elements);
	
private:
	std::string _name;
	std::string _last_selected_item = "";
};

}
