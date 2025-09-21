#pragma once 
#include <functional>
#include <mutex>
#include <thread>
#include <atomic>

namespace thread::main_thread 
{
	void add_event_and_wait(std::function<void()> event);
	void initialize();
	void update();
}
