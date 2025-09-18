#pragma once 
#include <string>

namespace editor::helpers::progress::popup
{

void initialize(const std::string& process_name);
void open();	

void update();

bool finsihed();
void finalize();

}
