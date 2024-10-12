#pragma once

namespace Waola {

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	class Observer
	{
	public:
		TUnsubscribeToken Subscribe(const TCallback& callback);
		void Unubscribe(const TUnsubscribeToken& token);
		void Notify(const TEvent& event);

	private:
		std::forward_list<TCallback> subscribers;
	};

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	TUnsubscribeToken Observer<TCallback, TUnsubscribeToken, TEvent>::Subscribe(const TCallback& callback)
	{
		auto unsubscribeToken = subscribers.before_begin();
		subscribers.push_front(callback);

		return unsubscribeToken;
	}

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	void Observer<TCallback, TUnsubscribeToken, TEvent>::Unubscribe(const TUnsubscribeToken& token)
	{
		subscribers.erase_after(token);
	}

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	void Observer<TCallback, TUnsubscribeToken, TEvent>::Notify(const TEvent& event)
	{
		for (auto& callback : subscribers) {
			callback(event);
		}
	}
}
