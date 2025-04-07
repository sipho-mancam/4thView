#include "kevents.hpp"

namespace KEvents
{
	EventsManager::EventsManager(std::string consumerTopic, std::string service_name, ulong poolSize, std::shared_ptr<spdlog::logger>logger)
		: worker(nullptr),
		logger(logger),
		frameCounter(0)
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
		if (!worker && !sync){
			worker = std::make_unique<std::thread>(&EventsManager::__run, this);
		}else {
			__run();
		}
			
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
	
	void EventsManager::sendEvent(std::string topic_name, Event e)
	{
		if (eventProducerPtr)
		{
			eventProducerPtr->sendMessage(topic_name, e);
		}
	}

	void EventsManager::__run()
	{
		
		while (!exitFlag)
		{
			long long elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(execTimeEnd - execTimeStart).count();
			if (elapsedTime >= 1)
			{
				execTimeStart = std::chrono::steady_clock::now();
				double frameRate = (frameCounter * 1.0) / (elapsedTime * 1.0);
				KEvents::kEventsLogger->info("Frame rate:{} fps ... elapsed Time: {} seconds.", frameRate, elapsedTime);
				frameCounter = 0;
			}
			Event currentEvent = eventConsumerPtr->update();
			
			if (currentEvent.getEventName() == EN_ERROR)
				continue;

			executorTreePtr->enqueueEvent(currentEvent);
			//this->sendEvent("sport_event_processor_mod_stream", currentEvent);
			frameCounter += 1;
			execTimeEnd = std::chrono::steady_clock::now();
			
			
		}
	}
	
	Event createEvent(std::string message)
	{
		Event e = Event::deserializeEvent(message);
		return e;
	}
}