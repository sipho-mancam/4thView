#pragma once
#include "kevents.hpp"


class OutputCallback : public KEvents::CallBackBase
{
public:
	OutputCallback(json globConfig, std::string servName);

	virtual void execute(KEvents::Event e) override;

private:
	json globalConfig;
	std::string serviceName, inputManagerTopic, inputManager;
};