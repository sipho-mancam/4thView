#include "executor.hpp"
#include "kafka.hpp"

namespace KEvents
{
	ExecutorTree::ExecutorTree(EventProducerPtr producer, ulong poolSize)
		:defaultExecutor()
	{
		eventProducer = producer;
		threadPool = std::make_shared<ThreadPool>(poolSize);
		defaultExecutor.setEventProducer(eventProducer);
		defaultExecutor.setThreadPool(threadPool);
	}
	ExecutorTree::~ExecutorTree()
	{
		kEventsLogger->info("Shutting down executor tree ...");
		exit();
	}
	void ExecutorTree::enqueueEvent(Event event)
	{
		execTimeStart = std::chrono::steady_clock::now();

		// Every event gets passed to the default executor
		defaultExecutor.executeEvent(event);
		execTimeEnd = std::chrono::steady_clock::now();

		long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(execTimeEnd - execTimeStart).count();
		//printf("Elapsed time to queue event: %lld ms\n", elapsedTime);
		// This is for specialized Executors
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
		
		std::vector<ExecutorBasePtr> execsList = executionTree[type];
		for (ExecutorBasePtr& exec : execsList)
		{
			if (exec == execPtr)
				return;
		}
		executionTree[type].push_back(execPtr);
	}

	void ExecutorTree::registerCallBack(std::string eventName, CallBackBasePtr _cbPtr)
	{
		defaultExecutor.registerCallBack(_cbPtr, eventName);
	}

	void ExecutorTree::exit()
	{
		if (threadPool)
			threadPool->stop();
	}

	ExecutorBase::ExecutorBase()
	{
		router = std::make_shared<RouterBase>();
		if (threadPool)
		{
			router->setThreadPool(threadPool);
		}

		if (eventProducer)
		{
			router->setEventProducer(eventProducer);
		}
	}

	void ExecutorBase::setEventProducer(EventProducerPtr eProducer)
	{
		eventProducer = eProducer;
		router->setEventProducer(eventProducer);
	}

	void ExecutorBase::setThreadPool(ThreadPoolPtr tPool)
	{
		threadPool = tPool;
		router->setThreadPool(threadPool);
	}

	void ExecutorBase::executeEvent(Event e)
	{
		/**
		* @note
		* Receive the event object, pass it to the router and return.
		*/
		//eventProducer->sendMessage("sport_event_processor_mod_stream", e);
		router->executeEvent(e);
	}
	void ExecutorBase::registerCallBack(CallBackBasePtr _cbPtr, std::string eventName)
	{
		router->registerCallback(_cbPtr, eventName);
	}

	void ExecutorBase::overwriteRouter(RouterPtr routerPtr)
	{
		router = routerPtr;
		if (threadPool)
			router->setThreadPool(threadPool);

		if (eventProducer)
			router->setEventProducer(eventProducer);
	}

	DefaultExecutor::DefaultExecutor()
		: ExecutorBase()
	{
	}
}