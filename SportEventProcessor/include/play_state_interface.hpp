#pragma once
#include "live_mode.hpp"
#include "replay_mode.hpp"
#include <memory>


class PlayerStateInteface
{
public:
	PlayerStateInteface() = default;
	~PlayerStateInteface() = default;

	virtual void setPlayStatePaused(bool paused);
	virtual bool getPlayStatePaused() const;

private:
	std::shared_ptr<LiveMode> m_playbackMode;
	std::shared_ptr<ReplayMode> m_replayMode;
	bool isLiveMode;

};
