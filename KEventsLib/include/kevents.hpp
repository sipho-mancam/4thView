#pragma once
#include <string>
#include <iostream>
#include <atomic>
#include <thread>
#include <nlohmann/json.hpp>

#include "kafka.hpp"
#include "executor.hpp"
#include "concurrent_queue.hpp"
#include "callback.hpp"

#include "types.hpp"

namespace KEvents
{

	Event createEvent(std::string message);


	/**
	* @brief 
	* This is the top level class, implementing a lot of the frameworks parts and
	* managing the Consumer as well as the event Q
	*/
	class EventsManager
	{
	public:
		EventsManager(std::string consumerTopic, ulong poolSize=3);
		~EventsManager();

		void startEventLoop();
		void exit();

	private:
		EventQueuePtr eventsQueue;
		std::shared_ptr<EventConsumer> eventConsumerPtr;
		std::shared_ptr<EventProducer> eventProducerPtr;
		ExecutorTreePtr executorTreePtr;
		std::atomic<bool> exitFlag;
		std::unique_ptr<std::thread> worker;
		virtual void __run();
	};
}