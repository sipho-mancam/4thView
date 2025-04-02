#pragma once
#include "live_mode.hpp"
#include "replay_mode.hpp"
#include "play_mode_manager.hpp"
#include "frame_output_event_dispatch.hpp"
#include "output_clock.hpp"
#include <memory>
#include "kevents.hpp"


class PlayerStateInteface
{
public:
	PlayerStateInteface(KEvents::EventsManager *evMan);
	~PlayerStateInteface() = default;

	virtual void setPlayStatePaused(bool paused);
	void setLiveMode(bool liveMode);
	void appendFrame(json frame);

private:
	std::shared_ptr<LiveMode> m_liveMode;
	std::shared_ptr<ReplayMode> m_replayMode;
	std::shared_ptr<PlayModeManager> m_playModeManager;
	std::shared_ptr<FrameOutputEventDispatch> m_frameOutputEventDispatch;
	std::shared_ptr<OutputClock> m_outputClock;
	KEvents::EventsManager* eventsManager;
	bool isLiveMode;

};
