#include "play_state_interface.hpp"

PlayerStateInteface::PlayerStateInteface(KEvents::EventsManager* evMan, std::shared_ptr<EventDataStore> evDs)
	:eventsManager(evMan),
	isLiveMode(true),
	m_eventDataStore(evDs)
{
	m_liveMode = std::make_shared<LiveMode>();
	m_replayMode = std::make_shared<ReplayMode>(m_eventDataStore);
	m_playModeManager = std::make_shared<PlayModeManager>(m_liveMode, m_replayMode);

	m_frameOutputEventDispatch = std::make_shared<FrameOutputEventDispatch>(m_playModeManager, eventsManager);
	m_outputClock = std::make_shared<OutputClock>(m_frameOutputEventDispatch);
}

void PlayerStateInteface::setPlayStatePaused(bool paused)
{
	m_liveMode->setPlayStatePaused(paused);
	m_replayMode->setPlayStatePaused(paused);
}

void PlayerStateInteface::setLiveMode(bool liveMode)
{
	m_playModeManager->setLiveMode(liveMode);
}

void PlayerStateInteface::switchToStoredStatePlayback(std::string eventName)
{
	m_playModeManager->setLiveMode(false);
	m_replayMode->switchToStoredPlayback(eventName);
}

void PlayerStateInteface::appendFrame(json frame)
{
	m_liveMode->appendFrame(frame);
	m_replayMode->appendFrame(frame);
	m_outputClock->externalFrameUpdate();
}

void PlayerStateInteface::setSeekerPosition(int position)
{
	m_replayMode->setSeekerPosition(position);
}
