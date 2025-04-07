#include "streamCallback.hpp"


void StreamDataCallback::execute(KEvents::Event e)
{
	static long long frameCounter = 0;
	//printf("\rFrame count: %lld ...", frameCounter);
	playState->appendFrame(e.getEventData());
	frameCounter += 1;
}