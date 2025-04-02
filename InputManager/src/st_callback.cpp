#include "st_callback.hpp"


StreamDataCallback::StreamDataCallback(std::string processor_topic, std::string servName)
	:processorTopic(processor_topic + TE_STREAM_EXT),
	serviceName(servName)
{
}

void StreamDataCallback::execute(KEvents::Event e)
{
	static long long frameCounter = 0;
	// pass the input through to the SportEventProcessor
	if (eventProducerPtr)
	{
		e.setSourceModule(serviceName);
		eventProducerPtr->sendMessage(processorTopic, e);
		//printf("\rFrame count: %lld ...", frameCounter);
		frameCounter += 1;
	}
}
