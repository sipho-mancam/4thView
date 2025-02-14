#pragma once

#include "kevents.hpp"


class TrackerConsumer
{
public:
	TrackerConsumer(json globConfig, std::string serviceName);
	bool waitForMessage();
	json getCurrentMessage();

private:
	json globalConfig;
	std::string serviceName;
	std::shared_ptr<RdKafka::KafkaConsumer> kConsumer;
	std::string currentMessage;
	bool messageStatus;
};