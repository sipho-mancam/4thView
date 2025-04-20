#include "frame_buffer.hpp"

FrameBuffer::FrameBuffer(std::shared_ptr<EventDataStore> evDs)
	: currentFrameIndex(0), 
	maxBufferSize(MAX_BUFFER_SIZE), 
	liveMode(true), 
	playbackPaused(false),
	currentlyActive(false),
	m_eventDataStore(evDs)
{
	json globalConfig = KEvents::__load_config__();
	maxBufferSize = globalConfig["SportEventProcessor"]["buffer_length"];
	frameStore = std::make_shared<persistentFrameStore>();
}

FrameBuffer::FrameBuffer()
	: currentFrameIndex(0),
	maxBufferSize(MAX_BUFFER_SIZE),
	liveMode(true),
	playbackPaused(false),
	currentlyActive(false)
{
	json globalConfig = KEvents::__load_config__();
	maxBufferSize = globalConfig["SportEventProcessor"]["buffer_length"];
	frameStore = std::make_shared<persistentFrameStore>();
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::pushFrame(json frame)
{

	// In transient state where the frame buffer has not reached it's max capacity
	/**
	* 1. We append a frame, and send the updated frame buffer size to update seeker interval
	*	a. In this state we have access to live frames, when you seek forward you'll get newer frames
	* 2. In steady state mode:
	*	a. The frame buffer is always full, and we just replace the last frame with the new one and delete the first one.
	*		if we are currently not the ones played.
	*	b. if we are currently active (replaying) then we keep the buffer as it is and we send the new 
	*		frames straight to the frame store, they will be retrieved when we switch but to inactive.
	*/
	// our in memory frame buffer is full, we need to store the frame in the persistent store.

	//we are in transient state.
	if (frames.size() < maxBufferSize)
	{
		frames.emplace_back(frame);
		return;
	}
	// we are in steady state, we need to replace the first frame with the new one.
	/**
	* In steady state we need to know if we are currently active, that way we send frames straight to the frame store
	* or we are currently inactive, that way we replace the first frame with the new one.
	*/
	if (currentlyActive)
	{
		frameStore->dumpFrame(frame);
	}
	else
	{
		frames.erase(frames.begin(), frames.begin() + 1);
		frames.emplace_back(frame);
	}
}

json FrameBuffer::currentFrame()
{
	if (frames.size() == 0)
	{
		return json();
	}

	if (currentFrameIndex >= frames.size())
	{
		currentFrameIndex = frames.size() - 1;
	}

	json currentFrame = frames[currentFrameIndex];
	if (currentlyActive && !playbackPaused)
	{
		currentFrameIndex += 1;
	}
	return currentFrame;
}

bool FrameBuffer::isEmpty()
{
	return frames.size() == 0;
}

void FrameBuffer::clear()
{
	frames.clear();
	currentFrameIndex = 0;
}

int FrameBuffer::size()
{
	return frames.size();
}

void FrameBuffer::setCurrentlyActive(bool mode)
{
	currentlyActive = mode;
	// if we are not currently active, we need to load the frames from the frame store
	if (!currentlyActive)
	{
		std::vector<json> storedFrames = frameStore->loadStore();
		// how far are we from buffer being full since our last live mode
		int startIndex = maxBufferSize - storedFrames.size();

		for (json frame : storedFrames)
		{
			frames.insert(frames.begin() + startIndex, frame);
			startIndex++;
		}
	}
	
}

void FrameBuffer::switchToStoreState(std::string eventName)
{
	// we are in replay mode now.
	currentlyActive = true;

	// we need to load the frames from the frame store
	std::vector<json> storedFrames = m_eventDataStore->loadEvent(eventName);
	if (storedFrames.size() == 0)
	{
		return;
	}
	int startIndex = maxBufferSize - storedFrames.size();

	for (json frame : storedFrames)
	{
		frames.insert(frames.begin() + startIndex, frame);
		startIndex++;
	}
}


