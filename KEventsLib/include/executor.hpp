#pragma once
#include "router.hpp"
#include <memory>
#include <map>
#include <vector>


namespace KEvents
{
	/**
	*@brief
	* The Executor tree is the heart of our events library, it implements the infrastructure
	* required to execute events as they come, by registering executors for the event types you are
	* interested in handling. It manages the ThreadPool runnning underneath that allows events to be
	* handled asynchronously, on separate threads.
	* @param EventProducerPtr producer - a shared_ptr to an event Producer, that will be passed to all children implementing the executor
	*
	*/
	class ExecutorTree
	{
	public:
		ExecutorTree(EventProducerPtr producer);
		void enqueueEvent(Event event);
		void registerExecutor(ExecutorBasePtr exec, EventType type);

	private:
		std::map<EventType, std::vector<ExecutorBasePtr>> executionTree;
	};

	/**
	* @brief
	*The ExecutorBase is the base class for all Executor classes, it's an abstract class and can't be 
	* instantiated on its own, it has to be sublcasses by an actual executor implementing the execute event.
	*/
	class ExecutorBase
	{
	public:
		ExecutorBase() = default;
		virtual void setEventProducer(EventProducerPtr eventProducer);
		virtual void setThreadPool(ThreadPoolPtr threadPool);
		/**
		* @brief 
		* The function always called by the execution tree to execute events.
		* this function must not under no circumstance execute any long standing tasks, 
		* it must always pass data to the router.
		*/
		virtual void executeEvent(Event e) final;

	private:
		EventProducerPtr eventProducer;
		Router router;
	};
}