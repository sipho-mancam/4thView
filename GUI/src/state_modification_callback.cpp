#include "state_modification_callback.hpp"

StateModificationCb::StateModificationCb(json globConfig, std::string servName, QObject* parent)
	:QObject(parent),
	globalConfig(globConfig),
	serviceName(servName)
{
	processor = "Processor";
}

void StateModificationCb::execute(KEvents::Event e)
{
}

void StateModificationCb::playerStateChanged(json stateObject)
{
	if (eventProducerPtr)
	{
		std::string procAddress = globalConfig[processor]["serviceTopic"];
		// create 3 events for hight, position, annotation
		if (stateObject.contains("highlight"))
		{
			KEvents::Event e;
			e.setEventName(EN_STATE_MOD);
			e.setSourceModule("gui");
			e.setEventType(KEvents::E_GUI);

			json stateEventObject;
			stateEventObject["state_def"] = KEvents::STATES_DEF::HIGHLIGHT;
			stateEventObject["data"] = stateObject["highlight"];
			e.setEventData(stateEventObject);
			eventProducerPtr->sendMessage(procAddress, e);
		}
		if (stateObject.contains("annotation"))
		{

			KEvents::Event e;

			e.setEventName(EN_STATE_MOD);
			e.setSourceModule("gui");
			e.setEventType(KEvents::E_GUI);

			json stateEventObject;
			stateEventObject["state_def"] = KEvents::STATES_DEF::ANNOTATION;
			stateEventObject["data"] = stateObject["annotation"];
			e.setEventData(stateEventObject);
			eventProducerPtr->sendMessage(procAddress, e);
		}

		if (stateObject.contains("position"))
		{
			KEvents::Event e;
			e.setEventName(EN_STATE_MOD);
			e.setSourceModule("gui");
			e.setEventType(KEvents::E_GUI);

			json stateEventObject;
			stateEventObject["state_def"] = KEvents::STATES_DEF::POSITION;
			stateEventObject["data"] = stateObject["position"];
			e.setEventData(stateEventObject);
			eventProducerPtr->sendMessage(procAddress, e);
		}

		if (stateObject.contains("team"))
		{
			KEvents::Event e;
			e.setEventName(EN_STATE_MOD);
			e.setSourceModule("gui");
			e.setEventType(KEvents::E_GUI);

			json stateEventObject;
			stateEventObject["state_def"] = KEvents::STATES_DEF::TEAM;
			stateEventObject["data"] = stateObject["team"];
			e.setEventData(stateEventObject);
			eventProducerPtr->sendMessage(procAddress, e);
		}
	}

	std::cout << "State Modified in some way ..." << std::endl;
}
