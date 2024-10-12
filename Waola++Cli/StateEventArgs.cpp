#include "pch.h"

#include "StateEventArgs.h"

using namespace WaolaCli;

StateEventArgs::StateEventArgs(const Waola::StateEvent& StateEvent)
{
	Tasks = (WaolaTask)StateEvent.Tasks;
}

WaolaTask StateEventArgs::Tasks::get()
{
	return tasks;
}

void StateEventArgs::Tasks::set(WaolaTask tasks)
{
	this->tasks = tasks;
}
