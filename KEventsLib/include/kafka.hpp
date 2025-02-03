#pragma once
#include <thread>
#include <atomic>

namespace KEvents
{
	/**
	*@brief
	*	EventProduce is a wrapper for the Kafka Producer class. It abstracts the exact underlying producer
	*	transport we use to send events to the external world, for our use, we'll implement the Kafka producers,
	*	but you can re-implement it and use a different outlet like Http, File Write or any other event mechanism 
	*	you want to produce to.
	*/
	class EventProducer
	{
	public:
		EventProducer();
	};

	/**
	*@brief
	*	EventConsumer is a wrapper for a kafka consumer, 
	*	it implements the thread and starts the consumer polling loop 
	*	on that execution thread, it's also responsible for killing the consumer.
	*/
	class EventConsumer
	{
	public:
		EventConsumer();
	};
}