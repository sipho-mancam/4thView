#include "kafka.hpp"

namespace KEvents
{

	std::shared_ptr<KafkaConsumer> buildConsumer(std::string broker, std::string group_id, ConsumeCb* _messageCb)
	{
		Conf* config = Conf::create(Conf::ConfType::CONF_GLOBAL);
		std::string err;
		if (config->set("bootstrap.servers", broker, err) != Conf::CONF_OK)
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

		if (_messageCb)
		{
			std::cout << "Registering message callback" << std::endl;
			if (config->set("consume_cb", _messageCb, err) != Conf::CONF_OK)
			{
				std::cerr << err << std::endl;
				throw std::runtime_error("Couldn't set message callback");
			}
		}

		KafkaConsumer* consumer = KafkaConsumer::create(config, err);
		std::shared_ptr<KafkaConsumer> sConsumer = std::shared_ptr<KafkaConsumer>(consumer);
	
		// how do we set up our callback system to receive messages as callbacks?
		return sConsumer;
	}


	EventProducer::EventProducer()
	{

	}

	
	EventConsumer::EventConsumer(std::string consumerTopic)
		: _callBackHandler()
	{
		kConsumer = buildConsumer("127.0.0.1:9092", "test_consumer", &_callBackHandler);
		kConsumer->subscribe(std::vector<std::string>({ consumerTopic }));
	}

	EventConsumer::~EventConsumer()
	{
		std::cout << "Shutting down events consumer" << std::endl;
		kConsumer->close();
		std::cout << "Shutting down events consumer complete" << std::endl;
	}

	void EventConsumer::update()
	{	
		kConsumer->poll(20);
	}

	void EventConsumer::subscribeEventsQueue(EventQueuePtr eq)
	{
		_callBackHandler.subscribeEventsQueue(eq);
	}


	void MessageCallback::consume_cb(Message& message, void* opaque)
	{
		if (message.err() == ERR_NO_ERROR)
		{
			std::string message_ = std::string(static_cast<const char*>(message.payload()), message.len());
			Event e = Event::deserializeEvent(message_);
			for (EventQueuePtr q : qsList)
			{
				q->push(e);
			}
		}
	}

	MessageCallback::~MessageCallback()
	{
		
	}
	void MessageCallback::subscribeEventsQueue(EventQueuePtr q)
	{
		qsList.push_back(q);
	}
}