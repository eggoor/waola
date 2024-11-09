#include "pch.h"

#include "include/EventBase.h"

using namespace Waola;

void EventBase::SetSubscriber(const void* subscriber)
{
	this->subscriber = subscriber;
}

const void* EventBase::GetSubscriber() const
{
	return subscriber;
}
