#pragma once
#include "kevents.hpp"
#include "frames_manager.hpp"
#include "play_state_interface.hpp"

class ControlCallback : public KEvents::CallBackBase
{
public:
	ControlCallback(json globalConfig, std::string moduleName, FramesManager& fm, PlayerStateInteface *psi);
	virtual void execute(KEvents::Event) override;
	~ControlCallback();

private:
	std::string moduleName;
	json globalConfig;
	json moduleConfig;
	FramesManager& framesManager;
	PlayerStateInteface* playStateInterface;

};