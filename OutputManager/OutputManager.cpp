#include <iostream>
#include "kevents.hpp"
#include "stream_cb.hpp"

int main()
{
	json config = KEvents::__load_config__();
	json moduleConfig = config["OutputManager"];
	json systemSettings = config["systemSettings"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::kEventsLogger->info("Starting {}", serviceName);
	KEvents::EventsManager eventsManager(serviceTopic, serviceName);

	std::shared_ptr<StreamCallback> streamCb = KEvents::createCallback<StreamCallback>(config, serviceName);

	eventsManager.registerCallback(EN_STREAM_DATA_UPDATE, streamCb);

	eventsManager.startEventLoop(true);

	return 0;
}
