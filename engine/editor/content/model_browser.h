#pragma once 
#include <string>
namespace editor::model_browser 
{
class model_browser 
{
public:
	void initialize();
	bool last_selected_is_empty() const { return _last_selected_model.empty(); }
	std::string last_selected_model() const { return _last_selected_model;}
	void update();
	
private:
	std::string _name;
	std::string _last_selected_model = "";
};
}
