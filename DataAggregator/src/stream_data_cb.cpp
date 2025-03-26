#include "stream_data_cb.hpp"

StreamCallback::StreamCallback(json globConf, std::string servName, std::shared_ptr<StateManager> stateMan)
	: globalConfig(globConf),
	serviceName(servName),
	stateManager(stateMan),
	streamCapture(false),
	streamPaused(false)
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
			static int frameCounter = 0;

			std::string sEPStreamTopic = globalConfig[sportEventProcessorName]["serviceTopic"];
			sEPStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(sEPStreamTopic, e);
			frameCounter += 1;

			if (frameCounter > 50)
			{
				streamCapture = false;
				frameCounter = 0;
				KEvents::Event ev;
				ev.setEventName(EN_STATE_CAPTURE_STOP);
				ev.setSourceModule(serviceName);
				eventProducerPtr->sendMessage(sEPStreamTopic, ev);
			}	
		}

		// Update the states that need updating
		currentFrame = e.getEventData();
		json updatedEventData = stateManager->updateTrackData(currentFrame);
		
		if (streamPaused && capturedFrame.empty())
		{
			capturedFrame = currentFrame;
		}

		e.setEventName(EN_STREAM_DATA_UPDATE);
		e.setSourceModule(serviceName);
		e.setEventData(updatedEventData);


		if (streamPaused)
		{
			updatedEventData = stateManager->updateTrackData(capturedFrame);
			e.setEventData(updatedEventData);

			std::string omStreamTopic = globalConfig[outputManagerName]["serviceTopic"];
			omStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(omStreamTopic, e);

			std::string procStreamTopic = globalConfig[processorName]["serviceTopic"];
			procStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(procStreamTopic, e);
		}
		else {
			std::string omStreamTopic = globalConfig[outputManagerName]["serviceTopic"];
			omStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(omStreamTopic, e);

			std::string procStreamTopic = globalConfig[processorName]["serviceTopic"];
			procStreamTopic += TE_STREAM_EXT;
			eventProducerPtr->sendMessage(procStreamTopic, e);
		}

	}
	else if (e.getEventName() == EN_STATE_MOD)
	{
		stateManager->updateState(e.getEventData());
		std::cout << "Received State Modification event." << std::endl;
	}
	else if (e.getEventName() == EN_STATE_CAPTURE_START)
	{
		streamCapture = true;
		eventProducerPtr->sendMessage(globalConfig[sportEventProcessorName]["serviceTopic"], e);
		KEvents::kEventsLogger->info("Start stream Capture");
	}
	else if (e.getEventName() == EN_STATE_CAPTURE_STOP)
	{
		streamCapture = false;
		eventProducerPtr->sendMessage(globalConfig[sportEventProcessorName]["serviceTopic"], e);
	}
	else if (e.getEventName() == EN_STREAM_PAUSE)
	{
		streamPaused = true;
		KEvents::kEventsLogger->info("Output Stream pause on a single frame output.");
	}
	else if (e.getEventName() == EN_STREAM_RESUME) {
		streamPaused = false;
		capturedFrame = json();
		KEvents::kEventsLogger->info("Output Stream resumed ...");
	}
}
