#pragma once
#include <nlohmann/json.hpp>
#include <iostream>

#include "concurrent_queue.hpp"

using json = nlohmann::json;

namespace KEvents
{
	/**
	* @brief
	* Forward declarations for most of the defined types in seperate files
	* 
	*/
	class CallBackBase;
	class ExecutorBase;
	class ExecutorTree;
	class EventProducer;
	class EventConsumer;
	class ThreadPool;
	class RouterBase;
	class RunnableThread;
	class DefaultExecutor;
	struct Event;

	using EventQueue = concurrent::ConcurrentQueue<Event>;
	using ExecutorBasePtr = std::shared_ptr<ExecutorBase>;
	using EventProducerPtr = std::shared_ptr<EventProducer>;
	using ThreadPoolPtr = std::shared_ptr<ThreadPool>;
	using CallBackBasePtr = std::shared_ptr<CallBackBase>;
	using RunnableThreadPtr = std::shared_ptr<RunnableThread>;
	using RouterPtr = std::shared_ptr<RouterBase>;
	using EventQueuePtr = std::shared_ptr<EventQueue>;
	using ExecutorTreePtr = std::shared_ptr<ExecutorTree>;
	using Router = RouterBase;

	typedef unsigned __int64 ulong;
	typedef unsigned int uint;

	/**
	* @brief
	* Any module implementing the executor tree uses this model to decide how to router events to their respective executors.
	*/ 
	enum EventType
	{
		DEFAULT = -1, // ignored events.
		E_GUI, // GUI related events
		E_DATA_AGGREGATOR,// data aggregator related events
		E_INPUT, // input related events
		E_PROCESSOR, // processor events
		E_SPORT_EVENT_PROCESSOR, // sport_event_processor related events
		E_OUTPUT, // Output related events
		E_MISC_EVENTS // Arbitrary modules related events.
	};

	struct Event
	{
	public:
		long eventId; // unique ID generated for the event.
		EventType eventType; // The type of module that created this event
		std::string sourceModule; // The module name that generated this event.
		std::string eventName; // This is used by the router to understand which callback to invoke for this particular event.
		json eventData;

		Event();
		Event(const Event& obj); // Copy Constructor
		Event(Event&& obj) noexcept; // Move Constructor deleted
		Event& operator=(const Event& obj); // Copy Assignable
		~Event() = default;

		long getEventID() { return eventId; }
		EventType getEventType() { return eventType; }
		std::string getSourceModuleName() { return sourceModule; }
		std::string getEventName() { return eventName; }
		json getEventData() { return eventData; }
		json getEventAsJson();

		std::string serializeEvent();
		void __deserializeEvent(std::string& str);

		static std::string serializeEvent(json  event);
		static Event deserializeEvent(std::string e);
	};
}

