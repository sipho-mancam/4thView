#pragma once
#include <nlohmann/json.hpp>
#include <mutex>
#include "kevents.hpp"

using json = nlohmann::json;

class PlaybackModeBase
{
public:
	PlaybackModeBase() = default;
	virtual ~PlaybackModeBase() = default;

	virtual void setPlayStatePaused(bool paused)
	{
		if(paused)
			KEvents::kEventsLogger->info("Playback paused");
		else
			KEvents::kEventsLogger->info("Playback resumed");

		m_playStatePaused = paused;
	}

	bool getPlayStatePaused() const
	{
		return m_playStatePaused;
	}

	virtual json getCurrentFrame() = 0;
	virtual void appendFrame(json frame) = 0;
	virtual void setCurrentlyActive(bool mode) { currentlyActive = mode; }

protected:
	bool m_playStatePaused = false, currentlyActive;
	std::mutex mux;
};