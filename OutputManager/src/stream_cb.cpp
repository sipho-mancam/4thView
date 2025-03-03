#include "stream_cb.hpp"

StreamCallback::StreamCallback(json globConfig, std::string servName)
	: globalConfig(globConfig),
	serviceName(servName)
{
	unrealConnector = "UnrealEngineConnector";
}

void StreamCallback::execute(KEvents::Event e)
{
	e.setSourceModule(serviceName);
	if (e.getEventName() == EN_STREAM_DATA_UPDATE)
	{
		std::string uEStreamTopic = globalConfig[unrealConnector]["serviceTopic"];
		uEStreamTopic += TE_STREAM_EXT;
		eventProducerPtr->sendMessage(uEStreamTopic, e);
	}
}