#include "stream_data_cb.hpp"

StreamCallback::StreamCallback(json globConf, std::string servName)
	: globalConfig(globConf),
	serviceName(servName)
{
}

void StreamCallback::execute(KEvents::Event e)
{

}
