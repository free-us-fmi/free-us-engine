#pragma once 

namespace editor::scene 
{

struct editor_init_data 
{
	int _frame_buffer_width;
	int _frame_buffer_height;
};

void initialize(const editor_init_data& data);
void clear();
void bind_framebuffer();
void update();
unsigned int get_framebuffer();

}
