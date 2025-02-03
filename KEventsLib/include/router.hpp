#pragma once
#include "callback.hpp"
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

	private:
		std::map<std::string, std::vector<CallBackBasePtr>> routingMap;
	};
}