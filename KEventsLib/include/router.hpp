#pragma once
#include "callback.hpp"
#include "thread_pool.hpp"
#include <map>
#include <vector>


namespace KEvents 
{
	class RouterBase final
	{
	public:
		RouterBase() = default;

		virtual void executeEvent(Event e) final;
		/**
		* @brief
		* This function creates the 'routes' that the Router can handle, but registering callbacks for routers.
		*/
		virtual void registerCallback(CallBackBasePtr _cbPtr, std::string& eventName) final;
		virtual void setThreadPool(ThreadPoolPtr threadPoolPtr);
		virtual void setEventProducer(EventProducerPtr eventProducer);
	private:
		std::map<std::string, std::vector<CallBackBasePtr>> routingMap;
		ThreadPoolPtr threadPoolPtr;
		EventProducerPtr eventProducer;
	};
}