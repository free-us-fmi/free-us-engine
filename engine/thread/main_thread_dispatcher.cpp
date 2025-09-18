#include "main_thread_dispatcher.h"
#include <queue>
#include <mutex>

namespace thread::main_thread 
{

namespace {
	std::queue<std::pair<std::function<void()>, std::mutex&>> _events;
	std::thread::id _main_thread_id;
	std::mutex _mutex;
}

void add_event(std::function<void()> event, std::mutex& mt, std::thread::id caller_thread)
{
	std::lock_guard<std::mutex> lg(_mutex);
	if ( caller_thread == _main_thread_id )
	{
		event();
		mt.unlock();
		return;
	}
	bool res = mt.try_lock();
	_events.emplace(event, mt);
}

void initialize()
{
	_main_thread_id = std::this_thread::get_id();
}

void update()
{
	while ( !_events.empty() )
	{
		auto& event = _events.front();
		event.first();
		event.second.unlock();
		_events.pop();
	}
}

}
