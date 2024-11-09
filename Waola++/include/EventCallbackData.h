/** @file EventCallbackData.h
* Waola event callback data.
*/

#pragma once

namespace Waola {
	template<typename TCallback, typename TEvent>
	class EventCallbackData
	{
	public:
		EventCallbackData<TCallback>(const void* subscriber, TCallback callback);
		void Raise(TEvent& event);
		
	private:
		const void* subscriber;
		TCallback callback;
	};

	template<typename TCallback, typename TEvent>
	EventCallbackData<TCallback, TEvent>::EventCallbackData(const void* subscriber, TCallback callback)
	: subscriber(subscriber)
	, callback(callback)
	{
	}

	template<typename TCallback, typename TEvent>
	void EventCallbackData<TCallback, TEvent>::Raise(TEvent& event)
	{
		event.SetSubscriber(subscriber);
		callback(event);
	}
}
