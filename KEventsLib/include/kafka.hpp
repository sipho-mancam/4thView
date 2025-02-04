#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <iostream>
#include <exception>
#include <librdkafka/rdkafkacpp.h>

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
	template<typename T>
	std::shared_ptr<KafkaConsumer> buildConsumer(std::string broker, std::string group_id)
	{
		Conf* config = Conf::create(Conf::ConfType::CONF_GLOBAL);
		std::string err;
		if (config->set("bootstrap.server", broker, err) != Conf::CONF_OK)
		{
			std::cerr << err << std::endl;
			throw std::runtime_error("Invalid Kafka Config for Bootsrap Server");
		}
		
		if (config->set("group.id", group_id, err) != Conf::CONF_OK)
		{
			std::cerr << err << std::endl;
			throw std::runtime_error("Invalid Kafka Config for Group Id");
		}

		if (config->set("auto.offset.reset", "latest", err) != Conf::CONF_OK)
		{
			std::cerr << err << std::endl;
			throw std::runtime_error("Invalid kafka Config for Auto Offset Reset");
		}

		KafkaConsumer* consumer = KafkaConsumer::create(conf, err);

		std::shared_ptr<KafkaConsumer> sConsumer = std::shared_ptr<KafkaConsumer>(consumer);

		// how do we set up our callback system to receive messages as callbacks?
		return sConsumer;
	}

	class MessageCallback : ConsumeCb
	{
		virtual void consume_cb(Message& message, void* opaque) override;

		virtual ~MessageCallback();
	};



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
		EventConsumer(std::string consumerTopic);
	};
}