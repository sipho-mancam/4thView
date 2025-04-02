#include "play_state_interface.hpp"

PlayerStateInteface::PlayerStateInteface(KEvents::EventsManager* evMan)
	:eventsManager(evMan),
	isLiveMode(true)
{
	m_liveMode = std::make_shared<LiveMode>();
	m_replayMode = std::make_shared<ReplayMode>();
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

void PlayerStateInteface::appendFrame(json frame)
{
	m_liveMode->appendFrame(frame);
	m_replayMode->appendFrame(frame);
	m_outputClock->externalFrameUpdate();
}
