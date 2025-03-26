#include "state_capture_callback.hpp"
#include <Windows.h>

StateCapture::StateCapture(json globConf, std::string modName)
	:CallBackBase(),
	globalConfig(globConf),
	moduleName(modName)
{
	processorTopic = globalConfig["Processor"]["serviceTopic"];
	moduleConfig = globalConfig[moduleName];
	const char dirPath[MAX_PATH] = { 0, };

	if (GetTempPath2A(MAX_PATH, (LPSTR)&dirPath))
	{
		KEvents::kEventsLogger->info("Temp Directory is: {}", dirPath);
		eventDataManager = std::make_shared<EventDataStore>(dirPath);
	}
	else {
		KEvents::kEventsLogger->error("Failed to find path to the Temp Directorya");
		throw std::exception("Failed to find the Temp Directory for data store");
	}
}

void StateCapture::execute(KEvents::Event e)
{
	if (e.getEventName() == EN_STATE_CAPTURE_START)
	{
		KEvents::kEventsLogger->info("Started capture event");
		json data = e.getEventData();
		eventDataManager->startCapture(data);
		
	}
	else if (e.getEventName() == EN_STATE_CAPTURE_STOP)
	{
		eventDataManager->stopCapture();
	}
	else if (e.getEventName() == EN_STREAM_DATA)
	{
		eventDataManager->capture(e.getEventData());
		//do this to test the module shift
	}
}

StateCapture::~StateCapture()
{
	eventDataManager.reset();
}
