#pragma once
#include "kevents.hpp"


class StreamCallback : public KEvents::CallBackBase
{
public:
	StreamCallback(json globConfig, std::string servName);

	virtual void execute(KEvents::Event e) override;

private:
	json globalConfig;
	std::string serviceName;

};