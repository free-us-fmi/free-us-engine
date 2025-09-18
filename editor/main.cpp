#include "application.h"
#include <chrono>
#include <memory>
#include <thread>
#include <iostream>
#include "editor.h"

int main() {
  
	
	std::unique_ptr<application> _application;
	_application = std::make_unique<application>();

	if ( !_application->Initialize() )
	{
		std::cout << "Failed to initialize application!" << std::endl;
		return 0;
	}

	editor::editor_init_data editor_descriptor;
	editor_descriptor._window = _application->get_window();
	editor_descriptor._scene_view_data._frame_buffer_height = 1080;
	editor_descriptor._scene_view_data._frame_buffer_width = 1920;
	editor_descriptor.program = _application->get_program();
	editor::initialize(editor_descriptor);
	_application->set_fbo(editor::scene::get_framebuffer());

	const double FPS = 120000.0;
	double frame_duration = 1. / FPS;

	double counter_start = glfwGetTime();
	int frame_count = 0;

	while ( ! _application->IsFinished() )
	{	
		editor::start_frame();
		double frame_start = glfwGetTime();
		_application->Run();
		double frame_end = glfwGetTime();
		double frame_difference = frame_end - frame_start;

		if ( frame_difference < frame_duration 	)
			std::this_thread::sleep_for(std::chrono::milliseconds(int(frame_duration * 1000 - frame_difference * 1000)));
		frame_count ++;

                if (frame_end - counter_start > 1.0) {
                  std::cout << "FPS: " << frame_count << std::endl;
                  frame_count = 0;
                  counter_start = frame_end;
                }
		editor::update();
		editor::draw();
        }

	_application->Finalize();

  	return 0;
}
