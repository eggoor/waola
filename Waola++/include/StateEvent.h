/** @file StateEvent.h
* Waola scanner state event.
*/

#pragma once

#include "Task.h"
#include "EventBase.h"

namespace Waola {

	class StateEvent: public EventBase
	{
	public:
		StateEvent(Task tasks);
		const Task Tasks;
		
	public:
		~StateEvent() = default;

	private:
		StateEvent(const StateEvent&) = delete;
		StateEvent(StateEvent&&) = delete;
		StateEvent& operator=(const StateEvent&) = delete;
		StateEvent& operator=(StateEvent&&) = delete;
	};
}
