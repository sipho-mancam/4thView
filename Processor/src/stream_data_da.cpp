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
			std::string iServName = globalConfig[inputManager]["serviceName"];
			std::string dServName = globalConfig[dataAggregator]["serviceName"];
			if (e.getSourceModuleName() == serviceName)
			{
				eventProducerPtr->sendMessage(globalConfig[dataAggregator]["serviceTopic"], e);
			}
			else if (e.getSourceModuleName() == dServName)
			{
				eventProducerPtr->sendMessage(globalConfig[gui]["serviceTopic"], e);
			}
		}
		else if(e.getEventName() == EN_STATE_MOD)
		{
			eventProducerPtr->sendMessage(globalConfig[dataAggregator]["serviceTopic"], e);
		}
	}
}