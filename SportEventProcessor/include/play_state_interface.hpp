#pragma once
#include "live_mode.hpp"
#include "replay_mode.hpp"
#include "play_mode_manager.hpp"
#include "frame_output_event_dispatch.hpp"
#include "output_clock.hpp"
#include <memory>
#include "kevents.hpp"
#include "event_data_store.hpp"

class PlayerStateInteface
{
public:
	PlayerStateInteface(KEvents::EventsManager *evMan, std::shared_ptr<EventDataStore> evDs);
	~PlayerStateInteface() = default;

	virtual void setPlayStatePaused(bool paused);
	void setLiveMode(bool liveMode);
	void appendFrame(json frame);
	void setSeekerPosition(int position);
	void switchToStoredStatePlayback(std::string eventName);

private:
	std::shared_ptr<LiveMode> m_liveMode;
	std::shared_ptr<ReplayMode> m_replayMode;
	std::shared_ptr<PlayModeManager> m_playModeManager;
	std::shared_ptr<FrameOutputEventDispatch> m_frameOutputEventDispatch;
	std::shared_ptr<OutputClock> m_outputClock;
	std::shared_ptr<EventDataStore> m_eventDataStore;
	KEvents::EventsManager* eventsManager;
	bool isLiveMode;

};
