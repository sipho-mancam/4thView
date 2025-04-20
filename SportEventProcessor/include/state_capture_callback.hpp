#pragma once
#include "kevents.hpp"
#include "event_data_store.hpp"

class StateCapture : public KEvents::CallBackBase
{
public:
	StateCapture(json globalConfig, std::string moduleName);

	virtual void execute(KEvents::Event) override;

	~StateCapture();
	std::shared_ptr<EventDataStore> GetEventDataStore() {
		return eventDataManager;
	}

private:
	std::string moduleName;
	json globalConfig;
	json moduleConfig;
	std::string rootWriteDir, processorTopic;
	std::shared_ptr<EventDataStore> eventDataManager;
};