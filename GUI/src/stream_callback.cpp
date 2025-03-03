#include "stream_callback.hpp"

StreamCallback::StreamCallback(json globConfig, std::string servName)
	: globalConfig(globalConfig),
	serviceName(servName)
{
}


void StreamCallback::execute(KEvents::Event e)
{
	for (auto _cb : registeredDataCallbacks)
	{
		_cb(e.getEventData());
	}
}


