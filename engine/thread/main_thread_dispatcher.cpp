#include "main_thread_dispatcher.h"
#include <queue>
#include <mutex>
#include <thread>
namespace thread::main_thread 
{

namespace {
	std::queue<std::pair<std::function<void()>, std::atomic_bool&>> _events;
	std::thread::id _main_thread_id;
	std::mutex _mutex;
}

void add_event_and_wait(std::function<void()> event)
{
	std::atomic_bool waiter = true;
	waiter.store(true);

	if ( std::this_thread::get_id() == _main_thread_id )
	{
		event();
		waiter.store(false);
		waiter.notify_all();
		return;
	}

	_mutex.lock();
	_events.emplace(event, waiter);
	_mutex.unlock();

	waiter.wait(true);
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
		event.second.store(false);
		event.second.notify_all();
		_events.pop();
	}
}

}
