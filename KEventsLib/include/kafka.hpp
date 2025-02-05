#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <exception>
#include <librdkafka/rdkafkacpp.h>
#include <vector>

#include "types.hpp"

using namespace RdKafka;

namespace KEvents
{
	/**
	*@brief
	*	EventProduce is a wrapper for the Kafka Producer class. It abstracts the exact underlying producer
	*	transport we use to send events to the external world, for our use, we'll implement the Kafka producers,
	*	but you can re-implement it and use a different outlet like Http, File Write or any other event mechanism 
	*	you want to produce to.
	*/
	std::shared_ptr<KafkaConsumer> buildConsumer(std::string broker, std::string group_id, ConsumeCb* _messageCb = nullptr);

	class MessageCallback : public ConsumeCb
	{
	public:
		virtual void consume_cb(Message& message, void* opaque) override;

		virtual ~MessageCallback();

		void subscribeEventsQueue(EventQueuePtr q);

	private:
		std::vector<EventQueuePtr> qsList;
	};



	class EventProducer
	{
	public:
		EventProducer();
	};

	/**
	*@brief
	*	EventConsumer is a wrapper for a kafka consumer, 
	*	it implements the kafka message callback and update the events queue subscribed,
	*	it's also responsible for killing the consumer.
	*/
	class EventConsumer
	{
	public:
		EventConsumer(std::string consumerTopic);
		~EventConsumer();
		void update();
		void subscribeEventsQueue(EventQueuePtr eq);

	private:

		std::shared_ptr<KafkaConsumer>kConsumer;
		MessageCallback _callBackHandler;
		EventQueuePtr eventQ;
	};
}