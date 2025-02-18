#include "stream_callback.hpp"

StreamCallback::StreamCallback(json globConfig, std::string servName)
	: globalConfig(globalConfig),
	serviceName(servName)
{
}


void StreamCallback::execute(KEvents::Event e)
{
	//std::cout << e.getEventName() << "\n" << std::flush;
	for (auto _cb : registeredDataCallbacks)
	{
		std::cout << "registered callback called \n";
		_cb(e.getEventData());
	}
}


