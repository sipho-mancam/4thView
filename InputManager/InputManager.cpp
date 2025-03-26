#include <iostream>
#include "kevents.hpp"
#include "st_callback.hpp"
#include <conio.h>



int main()
{
	json config = KEvents::__load_config__();
	json moduleConfig = config["InputManager"];
	json systemSettings = config["systemSettings"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::kEventsLogger->info("Starting {}", serviceName);
	KEvents::EventsManager eventsManager(serviceTopic, serviceName);

	std::shared_ptr<StreamDataCallback> streamCb = KEvents::createCallback<StreamDataCallback>(
		config["SportEventProcessor"]["serviceTopic"],
		serviceName);

	eventsManager.registerCallback(EN_STREAM_DATA, streamCb);

	eventsManager.startEventLoop(true);

	return 0;
	
}
