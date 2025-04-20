#pragma once
#include "types.hpp"
#include "kevents.hpp"
#include "persistentFrameStore.hpp"
#include "thread_pool.hpp"
#include "event_data_store.hpp"


class FrameBuffer
{
public:
	FrameBuffer(std::shared_ptr<EventDataStore> evDs);
	FrameBuffer();
	~FrameBuffer();
	void pushFrame(json frame);
	json currentFrame();

	bool isEmpty();
	void clear();
	int size();
	void setCurrentlyActive(bool mode); 
	void setPlaybackPaused(bool paused) { playbackPaused = paused; }
	void setCurrentFrameIndex(int index) { currentFrameIndex = index; }
	int getCurrentFrameIndex() { return currentFrameIndex; }
	bool isCurrentlyActive() { return currentlyActive; }
	void switchToStoreState(std::string eventName);

private:
	std::vector<json> frames;
	int currentFrameIndex, maxBufferSize;
	bool liveMode, playbackPaused, currentlyActive;
	std::shared_ptr<persistentFrameStore> frameStore;
	std::shared_ptr<EventDataStore> m_eventDataStore;
	KEvents::Lock lck;
};