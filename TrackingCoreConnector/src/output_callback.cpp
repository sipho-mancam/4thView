#include "output_callback.hpp"

OutputCallback::OutputCallback(json globConfig, std::string servName)
	: globalConfig(globConfig),
	serviceName(servName)
{
	inputManager = "InputManager";
	inputManagerTopic = globalConfig[inputManager]["serviceTopic"];
	inputManager += TE_STREAM_EXT;
}

void OutputCallback::execute(KEvents::Event e)
{
	if (eventProducerPtr)
	{
		eventProducerPtr->sendMessage(inputManagerTopic, e);
	}
}
