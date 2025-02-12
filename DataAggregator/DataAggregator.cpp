#include <iostream>
#include "kevents.hpp"
#include "state_management.hpp"
#include "stream_data_cb.hpp"

int main()
{
	json config = KEvents::__load_config__();
	json moduleConfig = config["DataAggregator"];
	json systemSettings = config["systemSettings"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::EventsManager eventsManager(serviceTopic, serviceName);
	std::shared_ptr<StateManager> stateManager = std::make_shared<StateManager>();
	
	std::shared_ptr<StreamCallback> streamCb = KEvents::createCallback<StreamCallback>(config, serviceName, stateManager);

	eventsManager.registerCallback(EN_STREAM_DATA, streamCb);
	eventsManager.registerCallback(EN_STATE_MOD, streamCb);
	eventsManager.registerCallback(EN_STATE_CAPTURE_START, streamCb);
	eventsManager.registerCallback(EN_STATE_CAPTURE_STOP, streamCb);

	eventsManager.startEventLoop(true);
	return 0;
}
