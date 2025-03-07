#include "router.hpp"


namespace KEvents
{
	
	/**
	* @brief 
	* This function receives the event object, and routes it to the registered
	* Callback functions, binding the registered callback with the event and passing it
	* to the thread pool as a task to execute.
	*/
	void RouterBase::executeEvent(Event e)
	{
		if (!threadPoolPtr)
		{
			kEventsLogger->error("No Thread Pool present in the Router ");
			return;
		}
			
		std::string eventName = e.getEventName();
		if (routingMap.contains(eventName))
		{
			// Create Task object to be passed to the thread pool for execution,
			std::vector<CallBackBasePtr>& callbacks = routingMap[eventName];
			for (CallBackBasePtr& _cbPtr : callbacks)
			{
				auto callableTask = std::bind(&CallBackBase::execute, _cbPtr, e);
				// Send thhe task off to the pool
				threadPoolPtr->appendTask(std::move(callableTask));
			}
		}
	}

	void RouterBase::registerCallback(CallBackBasePtr _cbPtr, std::string& eventName)
	{
		if(!routingMap.contains(eventName))
		{
			routingMap[eventName] = std::vector<CallBackBasePtr>();
		}
		// check if this callback doesn't already exist
		std::vector<CallBackBasePtr>& _cbs = routingMap[eventName];
		for (CallBackBasePtr& _cb : _cbs)
		{
			if (_cbPtr == _cb)
				return;
		}
		_cbPtr->setEventProducer(eventProducer);

		routingMap[eventName].push_back(_cbPtr);
	}

	void RouterBase::setThreadPool(ThreadPoolPtr tPool)
	{
		threadPoolPtr = tPool;
		
	}
	void RouterBase::setEventProducer(EventProducerPtr eProducer)
	{
		eventProducer = eProducer;
		for (auto item : routingMap)
		{
			std::vector<CallBackBasePtr>& cbs = item.second;
			for (CallBackBasePtr cb : cbs)
			{
				cb->setEventProducer(eventProducer);
			}
		}
	}
}

