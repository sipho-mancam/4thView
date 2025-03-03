#include "stream_data_da.hpp"

StreamDataCallback::StreamDataCallback(json globConf,  std::string servName)
	:globalConfig(globConf),
	serviceName(servName)
{
	inputManager = "InputManager";
	dataAggregator = "DataAggregator";
	gui = "GUI";
}

void StreamDataCallback::execute(KEvents::Event e)
{
	// pass the input through to the processor
	if (eventProducerPtr)
	{
		e.setSourceModule(serviceName);
		if (e.getEventName() == EN_STREAM_DATA) 
		{
			std::string daStreamTopic = globalConfig[dataAggregator]["serviceTopic"];
			daStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(daStreamTopic, e);
			
		}
		else if (e.getEventName() == EN_STREAM_DATA_UPDATE)
		{
			std::string guiStreamTopic = globalConfig[gui]["serviceTopic"];
			guiStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(guiStreamTopic, e);
		}else if (e.getEventName() == EN_STATE_MOD)
		{
			eventProducerPtr->sendMessage(globalConfig[dataAggregator]["serviceTopic"], e);
			std::cout << "Processor Received the state modification event" << std::endl;
		}
	}
}