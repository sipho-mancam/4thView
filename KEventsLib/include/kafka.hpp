#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <exception>
#include <librdkafka/rdkafkacpp.h>
#include <vector>
#include <time.h>

#include "types.hpp"

#define RETRY_COUNT 3
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
	std::shared_ptr<RdKafka::Producer> buildProducer(std::string broker, DeliveryReportCb* _deliveryCb = nullptr);

	class MessageCallback : public ConsumeCb
	{
	public:
		virtual void consume_cb(Message& message, void* opaque) override;

		virtual ~MessageCallback();

		void subscribeEventsQueue(EventQueuePtr q);

	private:
		std::vector<EventQueuePtr> qsList;
	};


	class MessageDeliveryCallback : public RdKafka::DeliveryReportCb
	{
	public:
		MessageDeliveryCallback();
		virtual void dr_cb(Message& message) override;
	};



	class EventProducer
	{
	public:
		EventProducer(std::string broker);
		~EventProducer();

		void sendMessage(std::string _topic, Event e);
		void sendMessage(std::string _topic, std::string _message);

	private:
		std::string __create_message__(Event e);
		std::string __create_message__(std::string e);

		std::shared_ptr<RdKafka::Producer> kafkaProducer;
		MessageDeliveryCallback deliveryReport;
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
		EventConsumer(std::string broker, std::string consumerTopic, std::string group_id);
		~EventConsumer();
		void update();
		void subscribeEventsQueue(EventQueuePtr eq);

	private:

		std::shared_ptr<KafkaConsumer>kConsumer;
		MessageCallback _callBackHandler;
		EventQueuePtr eventQ;
	};
}