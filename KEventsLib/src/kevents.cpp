#include "kevents.hpp"

namespace KEvents
{
	EventsManager::EventsManager(std::string consumerTopic, ulong poolSize)
		: worker(nullptr)
	{
		eventConsumerPtr = std::make_shared<EventConsumer>(consumerTopic);
		eventProducerPtr = std::make_shared<EventProducer>();

		executorTreePtr = std::make_shared<ExecutorTree>(eventProducerPtr, poolSize);

		eventsQueue = std::make_shared<EventQueue>();
	}

	EventsManager::~EventsManager()
	{
		exitFlag = true;
		std::cout << "Shutting down events manager ..." << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		if (worker->joinable())
			worker->join();
	}

	void EventsManager::startEventLoop()
	{
		worker = std::make_unique<std::thread>(&EventsManager::__run, this);
	}

	void EventsManager::exit()
	{
		exitFlag = true;
	}

	void EventsManager::__run()
	{
		while (!exitFlag)
		{
			if (eventsQueue->empty())
				continue;

			Event currentEvent = eventsQueue->pull();

			executorTreePtr->enqueueEvent(currentEvent);
		}
	}
}