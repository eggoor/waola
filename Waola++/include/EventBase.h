/** @file EventBase.h
* Waola++ event base.
*/

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
		
	public:
		virtual ~EventBase() = default;

	protected:
		EventBase() = default;

	private:
		EventBase(const EventBase&) = delete;
		EventBase(EventBase&&) = delete;
		EventBase& operator=(const EventBase&) = delete;
		EventBase& operator=(EventBase&&) = delete;
	};
}
