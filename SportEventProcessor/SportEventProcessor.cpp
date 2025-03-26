#include <iostream>
#include "kevents.hpp"
#include "state_capture_callback.hpp"
#include "frames_manager.hpp"
#include "streamCallback.hpp"
#include "control_callback.hpp"

int main()
{
	json config = KEvents::__load_config__();
	std::string moduleName = "SportEventProcessor";
	json moduleConfig = config[moduleName];
	json systemSettings = config["systemSettings"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::kEventsLogger->info("Starting {}", serviceName);
	KEvents::EventsManager eventsManager(serviceTopic, serviceName);
	FramesManager framesManager(eventsManager);

	std::shared_ptr<StateCapture> stateCapCb = KEvents::createCallback<StateCapture>(config, moduleName);
	eventsManager.registerCallback(EN_STATE_CAPTURE_START, stateCapCb);
	eventsManager.registerCallback(EN_STATE_CAPTURE_STOP, stateCapCb);
	eventsManager.registerCallback(EN_STATE_CAPTURE_LOAD, stateCapCb);
	eventsManager.registerCallback(EN_STREAM_DATA, stateCapCb);

	std::shared_ptr<StreamDataCallback> streamDataCb = KEvents::createCallback<StreamDataCallback>(config, moduleName, framesManager);
	eventsManager.registerCallback(EN_STREAM_DATA, streamDataCb);

	std::shared_ptr<ControlCallback> controlCb = KEvents::createCallback<ControlCallback>(config, moduleName, framesManager);
	eventsManager.registerCallback(EN_SET_STREAM_MODE, controlCb);
	eventsManager.registerCallback(EN_SET_SEEKER_POSITION, controlCb);
	eventsManager.registerCallback(EN_PLAYBACK_CONTROL, controlCb);

	eventsManager.startEventLoop(true);
	return EXIT_SUCCESS;
}

