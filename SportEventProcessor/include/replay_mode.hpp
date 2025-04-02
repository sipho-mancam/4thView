#pragma once
#include "mode_base.hpp"


class ReplayMode : public PlaybackModeBase
{
public:
	ReplayMode() = default;
	~ReplayMode() = default;
	json getCurrentFrame() override
	{
		return currentFrame;
	}
	void appendFrame(json frame) override
	{
		if (currentFrame.empty())
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