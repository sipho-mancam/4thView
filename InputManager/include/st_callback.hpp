#pragma once
#include "kevents.hpp"

using namespace KEvents;

class StreamDataCallback : public CallBackBase
{
public:
	StreamDataCallback(std::string processor_topic, std::string serviceName);

	virtual void execute(KEvents::Event e);


private:
	std::string processorTopic;
	std::string serviceName;
};