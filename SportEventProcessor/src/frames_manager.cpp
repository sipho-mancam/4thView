#include "frames_manager.hpp"
#include "kevents.hpp"

FramesManager::FramesManager(KEvents::EventsManager& evtPtr)
	: eventsManager(evtPtr)
{
	frameBuffer = std::make_shared<FrameBuffer>();
	globalConfig = KEvents::__load_config__();
	moduleConfig = globalConfig["SportEventProcessor"];
	processorTopic = globalConfig["Processor"]["serviceTopic"];
}

FramesManager::~FramesManager()
{
}

void FramesManager::pushFrame(json frame)
{
	//frameBuffer->pushFrame(frame);
	//if (frameBuffer->isLive())
	{
		json currentFrame = frame; // frameBuffer->currentFrame();
		sendFrameEvent(processorTopic, currentFrame);
	}
}

json FramesManager::currentFrame()
{
	return frameBuffer->currentFrame();
	
}

bool FramesManager::isEmpty()
{
	return frameBuffer->isEmpty();
}

void FramesManager::clear()
{
	frameBuffer->clear();
}

int FramesManager::size()
{
	return frameBuffer->size();
}

void FramesManager::setLiveMode(bool mode)
{
	frameBuffer->setCurrentlyActive(mode);
}

void FramesManager::setPlaybackPaused(bool paused)
{
	frameBuffer->setPlaybackPaused(paused);
}

void FramesManager::setSeekPosition(int position)
{
	frameBuffer->setCurrentFrameIndex(position);
}

void FramesManager::sendFrameEvent(std::string topic, json frame)
{
	KEvents::Event e;
	e.setSourceModule("SportEventProcessor");
	e.setEventName(EN_STREAM_DATA);
	e.setEventType(KEvents::EventType::E_SPORT_EVENT_PROCESSOR);
	e.setEventData(frame);
	eventsManager.sendEvent(topic, e);
}

void FramesManager::sendCurrentFrame()
{
	json currentFrame = frameBuffer->currentFrame();
	sendFrameEvent(processorTopic, currentFrame);
}
