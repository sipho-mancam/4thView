#include "stream_callback.hpp"

StreamCallback::StreamCallback(json globConfig, std::string servName)
	: globalConfig(globalConfig),
	serviceName(servName)
{
}


void StreamCallback::execute(KEvents::Event e)
{
	static long long frameCounter = 0;
	for (auto _cb : registeredDataCallbacks)
	{
		_cb(e.getEventData());
	}
	//std::cout << "\rFrame Count: " << frameCounter;
	frameCounter += 1;
}


