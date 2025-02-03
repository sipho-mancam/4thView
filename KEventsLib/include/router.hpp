#pragma once
#include "callback.hpp"
#include "thread_pool.hpp"
#include <map>
#include <vector>


namespace KEvents 
{
	class RouterBase
	{
	public:
		RouterBase();

		virtual void executeEvent(Event e) final;
		virtual void registerCallback(CallBackBasePtr _cbPtr, std::string& eventName) final;
		virtual void setThreadPool(ThreadPoolPtr threadPoolPtr);

	private:
		std::map<std::string, std::vector<CallBackBasePtr>> routingMap;
		ThreadPoolPtr threadPoolPtr;
	};
}