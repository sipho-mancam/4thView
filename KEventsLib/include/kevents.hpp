#pragma once
#include <string>
#include <iostream>
#include <atomic>
#include <thread>
#include <nlohmann/json.hpp>
#include <fstream>

#include "kafka.hpp"
#include "executor.hpp"
#include "concurrent_queue.hpp"
#include "callback.hpp"
#include "router.hpp"
#include "thread_pool.hpp"

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
		EventsManager(std::string consumerTopic, std::string service_name, ulong poolSize=3);
		~EventsManager();

		void startEventLoop(bool sync=false);
		void exit();
		void registerCallback(std::string eventName, CallBackBasePtr _cbPtr);
		void registerExecutor(EventType t, ExecutorBasePtr _executor);

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