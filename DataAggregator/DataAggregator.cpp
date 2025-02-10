#include <iostream>
#include "kevents.hpp"

int main()
{
	json config = KEvents::__load_config__();
	json moduleConfig = config["DataAggregator"];
	json systemSettings = config["systemSettings"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::EventsManager eventsManager(serviceTopic, serviceName);

	
	eventsManager.startEventLoop(true);
	return 0;
}
