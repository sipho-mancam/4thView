#pragma once
#include "kevents.hpp"


class StreamCallback :public KEvents::CallBackBase
{
public:
	StreamCallback(json globalConfig, std::string serviceName);
	virtual void execute(KEvents::Event e);

private:
	std::string vizConnector;
	std::string unrealConnector;
	json globalConfig;
	std::string serviceName;
};
