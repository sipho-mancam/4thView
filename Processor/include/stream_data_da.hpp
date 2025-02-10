#pragma once
#include "kevents.hpp"


class StreamDataCallback : public KEvents::CallBackBase
{
public:
	StreamDataCallback(json globalConfig, std::string serviceName);

	virtual void execute(KEvents::Event e);

private:
	std::string  serviceName;
	json globalConfig;
	std::string inputManager, dataAggregator, gui;

};