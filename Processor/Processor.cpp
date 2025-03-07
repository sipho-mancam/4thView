#include <iostream>

#include "kevents.hpp"
#include "stream_data_da.hpp"

int main()
{
	json config = KEvents::__load_config__();
	json moduleConfig = config["Processor"];
	json systemSettings = config["systemSettings"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::kEventsLogger->info("Starting {}", serviceName);
	KEvents::EventsManager eventsManager(serviceTopic, serviceName);

	std::shared_ptr<StreamDataCallback> streamCb = KEvents::createCallback<StreamDataCallback>(config, serviceName);

	// Stream Data Callbacks
	eventsManager.registerCallback(EN_STREAM_DATA, streamCb);
	eventsManager.registerCallback(EN_STREAM_DATA_UPDATE, streamCb);
	eventsManager.registerCallback(EN_STATE_MOD, streamCb);


	eventsManager.startEventLoop(true);
	return 0;
}

