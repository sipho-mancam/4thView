#include "control_callback.hpp"

ControlCallback::ControlCallback(json globalConfig, std::string moduleName, FramesManager& fm, PlayerStateInteface* psi)
	:
	moduleName(moduleName),
	globalConfig(globalConfig),
	framesManager(fm),
	playStateInterface(psi)
{
}

void ControlCallback::execute(KEvents::Event e)
{
	if (e.getEventName() == EN_PLAYBACK_CONTROL)
	{
		json eventData = e.getEventData();
		bool paused = eventData["playerbackpaused"];
		//framesManager.setPlaybackPaused(paused);
		playStateInterface->setPlayStatePaused(paused);
		KEvents::kEventsLogger->info("Playback control event received. Paused: {}", paused);
	}
	else if (e.getEventName() == EN_SET_STREAM_MODE)
	{
		json eventData = e.getEventData();
		bool liveMode = eventData["live_mode"];
		//framesManager.setLiveMode(liveMode);
		playStateInterface->setLiveMode(liveMode);
		KEvents::kEventsLogger->info("Stream mode set to live: {}", liveMode);
	}
	else if (e.getEventName() == EN_SET_SEEKER_POSITION)
	{
		json eventData = e.getEventData();
		int seekerPosition = eventData["seeker_position"];
		bool liveMode = eventData["live_mode"];
		/*framesManager.setSeekPosition(seekerPosition);
		framesManager.setLiveMode(liveMode);*/
		playStateInterface->setSeekerPosition(seekerPosition);
		KEvents::kEventsLogger->info("Seeker position set to: {}", seekerPosition);
	}
}

ControlCallback::~ControlCallback()
{

}
