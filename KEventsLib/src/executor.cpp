#include "executor.hpp"


namespace KEvents
{
	ExecutorTree::ExecutorTree(EventProducerPtr producer)
	{
	}
	void ExecutorTree::enqueueEvent(Event event)
	{
		EventType key = event.getEventType();
		if (executionTree.contains(key))
		{
			std::vector<ExecutorBasePtr>& executors = executionTree[key];
			for (ExecutorBasePtr& exec : executors)
			{
				exec->executeEvent(event);
			}
		}
	}

	void ExecutorTree::registerExecutor(ExecutorBasePtr execPtr, EventType type)
	{
		if (!executionTree.contains(type))
			executionTree[type] = std::vector<ExecutorBasePtr>();
		executionTree[type].push_back(execPtr);
	}

	
	void ExecutorBase::setEventProducer(EventProducerPtr eProducer)
	{
		eventProducer = eProducer;
	}

	void ExecutorBase::setThreadPool(ThreadPoolPtr threadPool)
	{

	}
	void ExecutorBase::executeEvent(Event e)
	{
		/**
		* @breif
		* Receive the event object, pass it to the router and return.
		*/


	}
}