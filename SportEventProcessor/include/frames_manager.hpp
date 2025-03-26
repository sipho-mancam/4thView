#pragma once
#include "kevents.hpp"
#include "frame_buffer.hpp"


class FramesManager {
public:
	FramesManager(KEvents::EventsManager& eventProd);
	~FramesManager();
	void pushFrame(json frame);
	json currentFrame();
	bool isEmpty();
	void clear();
	int size();
	void setLiveMode(bool mode);
	void setPlaybackPaused(bool paused); 
	void setSeekPosition(int position);
	void sendFrameEvent(std::string topic , json frame);
	void sendCurrentFrame();

private:
	std::shared_ptr<FrameBuffer> frameBuffer;
	KEvents::EventsManager& eventsManager;
	std::string processorTopic;
	json globalConfig, moduleConfig;
};