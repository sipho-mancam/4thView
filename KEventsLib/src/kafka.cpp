#include "kafka.hpp"

namespace KEvents
{
	EventProducer::EventProducer()
	{

	}




	
	EventConsumer::EventConsumer(std::string consumerTopic)
	{
	}


	void MessageCallback::consume_cb(Message& message, void* opaque)
	{

	}

	MessageCallback::~MessageCallback()
	{

	}
}