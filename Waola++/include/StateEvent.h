#pragma once

#include "Task.h"
#include "EventBase.h"

namespace Waola {

	class StateEvent: public EventBase
	{
	public:
		StateEvent(Task tasks);

		const Task Tasks;
	};
}
