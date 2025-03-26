#include "frame_buffer.hpp"

FrameBuffer::FrameBuffer()
	: currentFrameIndex(0), 
	maxBufferSize(MAX_BUFFER_SIZE), 
	liveMode(true), 
	playbackPaused(false)
{
	frameStore = std::make_shared<persistentFrameStore>();
}

FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::pushFrame(json frame)
{
	// our in memory frame buffer is full, we need to store the frame in the persistent store. if we are not in live mode
	if (frames.size() >= maxBufferSize)
	{
		if (liveMode)
		{
			frames.erase(frames.begin(), frames.begin() + 1);
			frames.emplace_back(frame);
			currentFrameIndex = currentFrameIndex > 0 ? currentFrameIndex - 1 : 0;
		}
		else { // we in replay mode
			frameStore->dumpFrame(frame);
			return;
		}
	}

	frames.push_back(frame);
	if (liveMode)
	{
		currentFrameIndex = frames.size() - 1;
	}
}

json FrameBuffer::currentFrame()
{
	if (liveMode)
	{
		if (frames.size() > 0) {
			json Frame = frames[currentFrameIndex];
			Frame["frame_index"] = currentFrameIndex;
			Frame["live_mode"] = liveMode;
			return Frame;

		} else {
			return json();
		}
	}else // we are in replay mode, and two things control the frame seeker, 1. playback pause state, 2. user seeker update.
	{
		if (frames.size() == 0)
		{
			return json();
		}

		if (currentFrameIndex >= frames.size())
		{
			currentFrameIndex = frames.size() - 1; // point to the last frame
		}

		json Frame = frames[currentFrameIndex];
		Frame["frame_index"] = currentFrameIndex;
		Frame["live_mode"] = liveMode;

		if (!playbackPaused)
		{
			currentFrameIndex += 1;
		}
		return Frame;
	}
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

void FrameBuffer::setLiveMode(bool mode)
{
	liveMode = mode;
	
	/**
	* On Live mode when we make the switch to go live, we need to do a few things:
	* 1. Get Frames from the frame store 
	* 2. Append the frames from the store to the current frame buffer, to have a continuations from the lost time
	* 3. 
	*/
	if (liveMode)
	{
		std::vector<json> storedFrames = frameStore->loadStore();
		// how far are we from buffer being full since our last live mode
		int startIndex = maxBufferSize - storedFrames.size();
		for (json frame : storedFrames)
		{
			frames.insert(frames.begin() + startIndex, frame);
		}
	}
}
