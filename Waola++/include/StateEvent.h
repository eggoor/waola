#pragma once

#include "Task.h"

namespace Waola {

	class StateEvent
	{
	public:
		StateEvent(Task tasks);

		const Task Tasks;
	};
}
