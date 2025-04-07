#pragma once
#include <nlohmann/json.hpp>
#include <mutex>

using json = nlohmann::json;

class PlaybackModeBase
{
public:
	PlaybackModeBase() = default;
	virtual ~PlaybackModeBase() = default;

	void setPlayStatePaused(bool paused)
	{
		m_playStatePaused = paused;
	}

	bool getPlayStatePaused() const
	{
		return m_playStatePaused;
	}

	virtual json getCurrentFrame() = 0;
	virtual void appendFrame(json frame) = 0;

protected:
	bool m_playStatePaused = false;
	std::mutex mux;
};