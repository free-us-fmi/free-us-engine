#pragma once 
#include <string>
namespace editor::helpers::progress 
{
	
class progress_bar
{
public:

	void initialize(const std::string& process_name);
	void update();
	std::string _process_name;
};

}
