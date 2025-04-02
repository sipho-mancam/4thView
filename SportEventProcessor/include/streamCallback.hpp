#pragma once
#include "types.hpp"
#include "kevents.hpp"
#include "frames_manager.hpp"
#include "play_state_interface.hpp"

class StreamDataCallback : public KEvents::CallBackBase
{
public:
	StreamDataCallback(json config, std::string modName, FramesManager& fm, PlayerStateInteface* psi)
		: CallBackBase(),
		globalConfig(config),
		moduleName(modName),
		framesManager(fm), 
		playState(psi)
	{
		processorTopic = globalConfig["Processor"]["serviceTopic"];
		moduleConfig = globalConfig["SportEventProcessor"];
	}

	virtual void execute(KEvents::Event e) override;


private:
	json globalConfig, moduleConfig;
	std::string moduleName, processorTopic;
	FramesManager& framesManager;
	PlayerStateInteface* playState;
};