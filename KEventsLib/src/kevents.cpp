#include "kevents.hpp"

namespace KEvents
{
	EventsManager::EventsManager(std::string consumerTopic, std::string service_name, ulong poolSize)
		: worker(nullptr)
	{
		json config = __load_config__();
		json& libConfig = config["kEventslib"];

		eventConsumerPtr = std::make_shared<EventConsumer>(libConfig["kafka"]["broker"], consumerTopic, service_name);
		eventProducerPtr = std::make_shared<EventProducer>(libConfig["kafka"]["broker"]);
		executorTreePtr = std::make_shared<ExecutorTree>(eventProducerPtr, poolSize);
		eventsQueue = std::make_shared<EventQueue>();
		eventConsumerPtr->subscribeEventsQueue(eventsQueue);
	}
	

	EventsManager::~EventsManager()
	{
		exitFlag = true;
		std::cout << "Shutting down events manager ..." << std::endl;
		exit();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (worker->joinable())
			worker->join();
		std::cout << "Events manager shutdown complete ... " << std::endl;
	}

	void EventsManager::startEventLoop(bool sync)
	{
		if (!worker && !sync)
			worker = std::make_unique<std::thread>(&EventsManager::__run, this);
		else
			__run();
	}

	void EventsManager::exit()
	{
		exitFlag = true;
		if (worker && worker->joinable())
		{
			worker->join();
		}
		executorTreePtr->exit();
	}

	void EventsManager::registerCallback(std::string eventName, CallBackBasePtr _cbPtr)
	{
		executorTreePtr->registerCallBack(eventName, _cbPtr);
	}

	void EventsManager::registerExecutor(EventType typ, ExecutorBasePtr _executor)
	{
		executorTreePtr->registerExecutor(_executor, typ);
	}

	void EventsManager::__run()
	{
		while (!exitFlag)
		{
			eventConsumerPtr->update();
			if (eventsQueue->empty())
			{
				// reduce the query overhead ...
				// Need to find a clever way to do this ...
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
				continue;
			}

			Event currentEvent = eventsQueue->pull();
#ifdef _DEBUG
			/*std::cout << currentEvent.getEventName() << std::endl;
			std::cout << currentEvent.getEventData() << std::endl;*/
#endif
			executorTreePtr->enqueueEvent(currentEvent);
		}
	}
	
	Event createEvent(std::string message)
	{
		Event e = Event::deserializeEvent(message);
		return e;
	}
}