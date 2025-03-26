#include "kafka.hpp"

namespace KEvents
{

	std::shared_ptr<KafkaConsumer> buildConsumer(std::string broker, std::string group_id, ConsumeCb* _messageCb)
	{
		Conf* config = Conf::create(Conf::ConfType::CONF_GLOBAL);
		std::string err;
		if (config->set("bootstrap.servers", broker, err) != Conf::CONF_OK)
		{
			kEventsLogger->error("Invalid Kafka Config for Bootsrap Server");
			throw std::runtime_error("Invalid Kafka Config for Bootsrap Server");
		}

		if (config->set("group.id", group_id, err) != Conf::CONF_OK)
		{
			kEventsLogger->error("{}",err);
			throw std::runtime_error("Invalid Kafka Config for Group Id");
		}

		if (config->set("auto.offset.reset", "latest", err) != Conf::CONF_OK)
		{
			kEventsLogger->error("{}", err);
			throw std::runtime_error("Invalid kafka Config for Auto Offset Reset");
		}

		if (config->set("enable.auto.commit", "false", err) != Conf::CONF_OK)
		{
			kEventsLogger->error("{}", err);
			throw std::runtime_error("Invalid kafka Config for Enable Auto Commit Offset");
		}

		if (_messageCb)
		{
			kEventsLogger->info("Registering kafka message callback");
			if (config->set("consume_cb", _messageCb, err) != Conf::CONF_OK)
			{
				kEventsLogger->error("{}", err);
				throw std::runtime_error("Couldn't set message callback");
			}
		}

		KafkaConsumer* consumer = KafkaConsumer::create(config, err);
		if (!consumer)
		{
			kEventsLogger->error("{} {}", "Failed to create consumer. Reason: ", err);
			throw std::runtime_error("failed to build consumer.");
		}
		std::shared_ptr<KafkaConsumer> sConsumer = std::shared_ptr<KafkaConsumer>(consumer);
		return sConsumer;
	}

	std::shared_ptr<RdKafka::Producer> buildProducer(std::string broker, DeliveryReportCb* _deliveryCb)
	{
		Conf* conf = Conf::create(RdKafka::Conf::CONF_GLOBAL);
		std::string err;

		if (conf->set("bootstrap.servers", broker, err) != RdKafka::Conf::CONF_OK)
		{
			kEventsLogger->error("Failed to set bootstrap.servers config");
			throw std::runtime_error("Failed to set bootstrap.servers config.");
		}
		if (_deliveryCb)
		{
			if (conf->set("dr_cb", _deliveryCb, err) != RdKafka::Conf::CONF_OK)
			{
				kEventsLogger->error( "Could not configure delivery callback" );
				kEventsLogger->error("Error: {}" , err );
			}
		}
		RdKafka::Producer* producer = RdKafka::Producer::create(conf, err);
		if (!producer)
		{
			kEventsLogger->error("Failed to create producer. Reason: {}", err );
			throw std::runtime_error("Failed to create event producer ...");
		}
		std::shared_ptr<RdKafka::Producer> sProducer = std::shared_ptr<RdKafka::Producer>(producer);
		return sProducer;
	}


	EventProducer::EventProducer(std::string broker)
		:
		deliveryReport()
	{
		kafkaProducer = buildProducer(broker, &deliveryReport);
	}

	EventProducer::~EventProducer()
	{
		kafkaProducer->flush(1000);
	}

	void EventProducer::sendMessage(std::string _topic, Event e)
	{
		if (!kafkaProducer)
			return;

		std::string message = __create_message__(e);
		ulong retryCount = 0;
	Retry:
		retryCount += 1;
		RdKafka::ErrorCode err = kafkaProducer->produce(
			_topic, // topic 
			RdKafka::Topic::PARTITION_UA, // partitiion flag
			RdKafka::Producer::RK_MSG_COPY, // message_flag
			const_cast<char*>(message.c_str()), // payload*
			message.size(), // payload_size
			const_cast<char*>(e.getSourceModuleName().c_str()),// Key
			e.getSourceModuleName().size(), // key size
			(int64_t)time(NULL), // time_stamp
			NULL // message_opaque
		);

		if (err != RdKafka::ERR_NO_ERROR && retryCount < RETRY_COUNT)
		{
			kafkaProducer->poll(100);
			goto Retry;
		}
		kafkaProducer->poll(10);

	}

	void EventProducer::sendMessage(std::string _topic, std::string _message)
	{
		if (!kafkaProducer)
			return;

		std::string message = __create_message__(_message);
		unsigned int retryCount = 0;

	Retry:
		retryCount += 1;
		RdKafka::ErrorCode err = kafkaProducer->produce(
			_topic, // topic 
			RdKafka::Topic::PARTITION_UA, // partitiion flag
			RdKafka::Producer::RK_MSG_COPY, // message_flag
			const_cast<char*>(message.c_str()), // payload*
			message.size(), // payload_size
			NULL,// Key
			0, // key size
			(int64_t)time(NULL), // time_stamp
			NULL // message_opaque
		);

		if (err != RdKafka::ERR_NO_ERROR && retryCount < RETRY_COUNT)
		{
			kafkaProducer->poll(100);
			goto Retry;
		}

		kafkaProducer->poll(10);
	}

	std::string EventProducer::__create_message__(Event e)
	{
		return e.serializeEvent();
	}

	std::string EventProducer::__create_message__(std::string e)
	{
		return e;
	}

	
	EventConsumer::EventConsumer(std::string broker, std::string consumerTopic, std::string group_id)
		: _callBackHandler()
	{
		kConsumer = buildConsumer(broker, group_id, &_callBackHandler);
		kConsumer->subscribe(std::vector<std::string>({ consumerTopic, consumerTopic + TE_STREAM_EXT }));
	}

	EventConsumer::~EventConsumer()
	{
		kEventsLogger->info("Shutting down events consumer");
		kConsumer->close();
		kEventsLogger->info("Shutting down events consumer complete");
	}

	void EventConsumer::update()
	{	
		kConsumer->poll(50);
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

	MessageDeliveryCallback::MessageDeliveryCallback()
		:RdKafka::DeliveryReportCb()
	{
	}

	void MessageDeliveryCallback::dr_cb(Message& message)
	{
		if (message.err() != RdKafka::ERR_NO_ERROR)
		{
			kEventsLogger->error("{}", message.errstr());
		}
	}
}