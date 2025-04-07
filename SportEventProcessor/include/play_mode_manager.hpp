#pragma once

#include "live_mode.hpp"
#include "replay_mode.hpp"
#include <iostream>
#include "kevents.hpp"


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

		m_liveMode->setCurrentlyActive(live_mode);
		m_replayMode->setCurrentlyActive(!live_mode);

		if (live_mode)
		{
			KEvents::kEventsLogger->info("Switched to Live Mode");
			currentMode = m_liveMode;
		}
		else
		{
			KEvents::kEventsLogger->info("Switched to Replay Mode");
			currentMode = m_replayMode;
		}
	}

	json getCurrentFrame()
	{
		return currentMode->getCurrentFrame();
	}

	int getCurrentSeekerPosition()
	{
		return m_replayMode->getCurrentSeekerPosition();
	}

	int getCurrentStoreSize()
	{
		return m_replayMode->getCurrentStoreSize();
	}
private:
	std::shared_ptr<LiveMode> m_liveMode;
	std::shared_ptr<ReplayMode> m_replayMode;
	std::shared_ptr<PlaybackModeBase> currentMode;
	std::mutex mux;
	bool live_mode;
};