#pragma once

#include "live_mode.hpp"
#include "replay_mode.hpp"
#include <iostream>


class PlayModeManager
{
public:
	PlayModeManager(std::shared_ptr<LiveMode> liveMode, std::shared_ptr<ReplayMode> replayMode)
		: m_liveMode(liveMode), 
		m_replayMode(replayMode), 
		live_mode(false)
	{
		currentMode = m_liveMode;
	}

	void setLiveMode(bool live)
	{
		live_mode = live;
		if (live_mode)
		{
			currentMode = m_liveMode;
		}
		else
		{
			currentMode = m_replayMode;
		}
	}

	json getCurrentFrame()
	{
		return currentMode->getCurrentFrame();
	}
private:
	std::shared_ptr<LiveMode> m_liveMode;
	std::shared_ptr<ReplayMode> m_replayMode;
	std::shared_ptr<PlaybackModeBase> currentMode;
	bool live_mode;
};