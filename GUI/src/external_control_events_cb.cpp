#include "external_control_events_cb.hpp"

ExternalGUIControlEvents::ExternalGUIControlEvents(QObject* parent)
	:QObject(parent),
	CallBackBase()
{
}

void ExternalGUIControlEvents::execute(KEvents::Event e)
{
	json evData = e.getEventData();
	if (e.getEventName() == EN_FRAME_STORE_SIZE)
	{
		int storeSize = evData["store_size"];
		Q_EMIT frameStoreSizeChanged(storeSize);
	}
	else if (e.getEventName() == EN_SET_SEEKER_POSITION)
	{
		int seekerPosition = evData["seeker_position"];
		Q_EMIT seekerPositionChanged(seekerPosition);
	}
}
