#include "router.hpp"


namespace KEvents
{
	RouterBase::RouterBase()
	{
	}
	/**
	* @brief 
	* This function receives the event object, and routes it to the registered
	* Callback functions, binding the registered callback with the event and passing it
	* to the thread pool as a task to execute.
	*/
	void RouterBase::executeEvent(Event e)
	{
		std::string eventName = e.getEventName();
		if (routingMap.contains(eventName))
		{
			// Create Task object to be passed to the thread pool for execution,
			std::vector<CallBackBasePtr>& callbacks = routingMap[eventName];
			for (CallBackBasePtr& _cbPtr : callbacks)
			{
				auto callableTask = std::bind(&CallBackBase::execute, _cbPtr, e);
				// threadPool.appendTask(callableTask);
			}
			// Send thhe task off to the pool

			// Leave
		}
	}
	void RouterBase::registerCallback(CallBackBasePtr _cbPtr, std::string& eventName)
	{
		if(!routingMap.contains(eventName))
		{
			routingMap[eventName] = std::vector<CallBackBasePtr>();
		}
		routingMap[eventName].push_back(_cbPtr);
	}
}

