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
		if (e.getEventName() == EN_STREAM_DATA || e.getEventName() == EN_STATE_MOD)
		{
			eventProducerPtr->sendMessage(globalConfig[dataAggregator]["serviceTopic"], e);
			/*if (e.getEventName() == EN_STATE_MOD)
				std::cout << "Recieved state modification event." << std::endl;*/
		}
		else if (e.getEventName() == EN_STREAM_DATA_UPDATE)
		{
			eventProducerPtr->sendMessage(globalConfig[gui]["serviceTopic"], e);
		}
	}
}