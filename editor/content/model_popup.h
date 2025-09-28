#pragma once 
#include <string>
#include "core/common.h"
#include <unordered_set>
#include "utility/vector.h"
namespace editor::model_browser::popup 
{
	void initialize();
	void update();
	std::string result();
	utl::vector<programs::program_id> program_result();
	void open();	
	bool finsihed();
	
}
