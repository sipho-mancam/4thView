#pragma once
#include "types.hpp"
#include "kevents.hpp"
#include "persistentFrameStore.hpp"
#include "thread_pool.hpp"



class FrameBuffer
{
public:
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

private:
	std::vector<json> frames;
	int currentFrameIndex, maxBufferSize;
	bool liveMode, playbackPaused, currentlyActive;
	std::shared_ptr<persistentFrameStore> frameStore;
	KEvents::Lock lck;
};