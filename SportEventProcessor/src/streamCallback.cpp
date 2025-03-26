#include "streamCallback.hpp"


void StreamDataCallback::execute(KEvents::Event e)
{
	framesManager.pushFrame(e.getEventData());
}