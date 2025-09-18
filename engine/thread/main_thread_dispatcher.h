#pragma once 
#include <functional>
#include <mutex>
#include <thread>
namespace thread::main_thread 
{
	void add_event(std::function<void()> event, std::mutex& mt, std::thread::id caller_thread);
	void initialize();
	void update();
}
