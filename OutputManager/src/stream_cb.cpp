#include "stream_cb.hpp"

StreamCallback::StreamCallback(json globConfig, std::string servName)
	: globalConfig(globConfig),
	serviceName(servName)
{
	unrealConnector = "UnrealEngineConnector";
	//std::cout << globalConfig << std::endl;
}

void StreamCallback::execute(KEvents::Event e)
{
	e.setSourceModule(serviceName);
	if (e.getEventName() == EN_STREAM_DATA_UPDATE)
	{
		eventProducerPtr->sendMessage(globalConfig[unrealConnector]["serviceTopic"], e);
	}
}