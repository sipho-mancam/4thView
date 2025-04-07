#pragma once
#include "mode_base.hpp"

class LiveMode : public PlaybackModeBase
{
public:
	LiveMode() = default;
	~LiveMode() = default;

	json getCurrentFrame() override
	{
		std::lock_guard<std::mutex>lck(mux);
		return currentFrame;
	}

	void appendFrame(json frame) override
	{
		std::lock_guard<std::mutex>lck(mux);
		if(currentFrame.empty())
		{
			currentFrame = frame;
			return;
		}

		if (!m_playStatePaused)
		{
			currentFrame = frame;
		}	
	}
private:
	json currentFrame;
};
