#include "kevents.hpp"

namespace KEvents
{
	EventsManager::EventsManager(std::string consumerTopic, std::string service_name, ulong poolSize, std::shared_ptr<spdlog::logger>logger)
		: worker(nullptr),
		logger(logger)
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
		logger->info("Shutting down events manager ..." );
		this->exit();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (worker->joinable())
			worker->join();
		logger->info("Events manager shutdown complete ... ");
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
				continue;
			}

			Event currentEvent = eventsQueue->pull();
			executorTreePtr->enqueueEvent(currentEvent);
		}
	}
	
	Event createEvent(std::string message)
	{
		Event e = Event::deserializeEvent(message);
		return e;
	}
}