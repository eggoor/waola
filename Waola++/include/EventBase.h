#pragma once

#include "Task.h"

namespace Waola {

	class EventBase
	{
	public:
		void SetSubscriber(const void* subscriber);
		const void* GetSubscriber() const;
		
	private:
		const void* subscriber;
	};
}
