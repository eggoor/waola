#pragma once

namespace Waola {

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	class Observer
	{
	public:
		TUnsubscribeToken Subscribe(const TCallback& callback);
		void Unubscribe(const TUnsubscribeToken& token);
		void Notify(TEvent& event);

	private:
		std::forward_list<TCallback> subscribtions;
	};

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	TUnsubscribeToken Observer<TCallback, TUnsubscribeToken, TEvent>::Subscribe(const TCallback& callback)
	{
		auto unsubscribeToken = subscribtions.before_begin();
		subscribtions.push_front(callback);

		return unsubscribeToken;
	}

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	void Observer<TCallback, TUnsubscribeToken, TEvent>::Unubscribe(const TUnsubscribeToken& token)
	{
		subscribtions.erase_after(token);
	}

	template<typename TCallback, typename TUnsubscribeToken, typename TEvent>
	void Observer<TCallback, TUnsubscribeToken, TEvent>::Notify(TEvent& event)
	{
		for (auto& subscribtion : subscribtions) {
			subscribtion.Raise(event);
		}
	}
}
