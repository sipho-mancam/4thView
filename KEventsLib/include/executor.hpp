#pragma once
#include "router.hpp"
#include "thread_pool.hpp"
#include <memory>
#include <map>
#include <vector>


namespace KEvents
{
	/**
	* @brief
	*The ExecutorBase is the base class for all Executor classes, it's an abstract class and can't be
	* instantiated on its own, it has to be sublcasses by an actual executor implementing the execute event.
	*/
	class ExecutorBase
	{
	public:
		ExecutorBase();

		virtual void setEventProducer(EventProducerPtr eventProducer);
		virtual void setThreadPool(ThreadPoolPtr threadPool);
		/**
		* @brief
		* The function that's always called by the execution tree to execute events.
		* this function must not under no circumstance execute any blocking tasks that take too long,
		* it must always pass data to the router and exit.
		* This function can not be override by the subclasses.
		*/
		virtual void executeEvent(Event e) final;

		/**
		* @brief
		* since the router itself is private to the children
		* This function exposes the registering interface of the router without exposing the router itself.
		*
		*/
		virtual void registerCallBack(CallBackBasePtr _cbPtr, std::string EventName);
		/**
		* @brief
		* Initializes the routes that specializes the executor, passing specific callbacks
		* via the registerCallback interface of the routes function
		*/
		virtual void initRoutes() = 0;

	protected:
		EventProducerPtr eventProducer;
		RouterPtr router;
		ThreadPoolPtr threadPool;
		// if the need ever comes for us to subclass the router and change stuff.
		virtual void overwriteRouter(RouterPtr routerPtr);
	};

	class DefaultExecutor : public ExecutorBase
	{
	public:
		DefaultExecutor();
		virtual void initRoutes() override {}
	};

	/**
	*@brief
	* The Executor tree is the heart of our events library, it implements the infrastructure
	* required to execute events as they come, by registering executors for the event types you are
	* interested in handling. It manages the ThreadPool runnning underneath that allows events to be
	* handled asynchronously, on separate threads.
	* @param EventProducerPtr producer - a shared_ptr to an event Producer, that will be passed to all children implementing the executor
	*
	*/
	class ExecutorTree final
	{
	public:
		ExecutorTree(EventProducerPtr producer, ulong poolSize=1);
		~ExecutorTree();

		void enqueueEvent(Event event);
		void registerExecutor(ExecutorBasePtr exec, EventType type);
		/**
		* @brief
		* This simplifies the process of registering event routes with their callback
		* Note: Any callback registered through this function will only be executed via the defaultExecutor,
		* For custom implementations of executors, you'll need to keep a reference to that executor to register callbacks
		* at a later stage and use the registerExecutor function instead.
		*/
		void registerCallBack(std::string eventName, CallBackBasePtr _cbPtr);
		void exit();

	private:
		std::map<EventType, std::vector<ExecutorBasePtr>> executionTree;
		DefaultExecutor defaultExecutor;
		ThreadPoolPtr threadPool;
		EventProducerPtr eventProducer;
	};
}