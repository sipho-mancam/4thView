#include "stream_data_cb.hpp"

StreamCallback::StreamCallback(json globConf, std::string servName, std::shared_ptr<StateManager> stateMan)
	: globalConfig(globConf),
	serviceName(servName),
	stateManager(stateMan),
	streamCapture(false)
{
	processorName = "Processor";
	sportEventProcessorName = "SportEventProcessor";
	outputManagerName = "OutputManager";
}



void StreamCallback::execute(KEvents::Event e)
{
	// for the stream data, first we check if it's tracking data.
	if (e.getEventName() == EN_STREAM_DATA)
	{
		if (streamCapture)
		{
			eventProducerPtr->sendMessage(globalConfig[sportEventProcessorName]["serviceTopic"], e);
		}
		// Update the states that need updating
		json eventData = e.getEventData();
		json updatedEventData = stateManager->updateTrackData(eventData);

		e.setEventName(EN_STREAM_DATA_UPDATE);
		e.setSourceModule(serviceName);
		e.setEventData(updatedEventData);

		eventProducerPtr->sendMessage(globalConfig[outputManagerName]["serviceTopic"], e);
		eventProducerPtr->sendMessage(globalConfig[processorName]["serviceTopic"], e);	

	}
	else if (e.getEventName() == EN_STATE_MOD)
	{
		stateManager->updateState(e.getEventData());
	}
	else if (e.getEventName() == EN_STATE_CAPTURE_START)
	{
		streamCapture = true;
		eventProducerPtr->sendMessage(globalConfig[sportEventProcessorName]["serviceTopic"], e);
	}
	else if (e.getEventName() == EN_STATE_CAPTURE_STOP)
	{
		streamCapture = false;
		eventProducerPtr->sendMessage(globalConfig[sportEventProcessorName]["serviceTopic"], e);
	}
}
