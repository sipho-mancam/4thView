#pragma once
#include "kevents.hpp"


class StreamCallback : public KEvents::CallBackBase
{
public:
	StreamCallback(json globConf, std::string servName);
	
	virtual void execute(KEvents::Event e) override;

private:
	std::string serviceName;
	json globalConfig;
};