#pragma once
#include "kevents.hpp"
#include "state_management.hpp"


class StreamCallback : public KEvents::CallBackBase
{
public:
	StreamCallback(json globConf, std::string servName, std::shared_ptr<StateManager> stateMan);
	virtual void execute(KEvents::Event e) override;

private:
	std::string serviceName, processorName, outputManagerName, sportEventProcessorName;
	json globalConfig;
	std::shared_ptr<StateManager> stateManager;
	bool streamCapture;
};