#pragma once
#include "types.hpp"
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
	void setLiveMode(bool mode); 
	void setPlaybackPaused(bool paused) { playbackPaused = paused; }
	void setCurrentFrameIndex(int index) { currentFrameIndex = index; }
	bool isLive() { return liveMode; }

private:
	std::vector<json> frames;
	int currentFrameIndex, maxBufferSize;
	bool liveMode, playbackPaused;
	std::shared_ptr<persistentFrameStore> frameStore;
	KEvents::Lock lck;
};