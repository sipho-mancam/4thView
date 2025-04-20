#pragma once
#include "mode_base.hpp"
#include "frames_manager.hpp"
#include "event_data_store.hpp"

class ReplayMode : public PlaybackModeBase
{
public:
	ReplayMode(std::shared_ptr<EventDataStore> evDs);
	~ReplayMode() = default;
	json getCurrentFrame() override
	{
		return m_frameBuffer->currentFrame();
	}
	void appendFrame(json frame) override
	{
		m_frameBuffer->pushFrame(frame);
	}

	virtual void setPlayStatePaused(bool paused) override
	{
		__super::setPlayStatePaused(paused);
		m_playStatePaused = paused;
		m_frameBuffer->setPlaybackPaused(paused);
	}

	int getCurrentSeekerPosition()
	{
		return m_frameBuffer->getCurrentFrameIndex();
	}

	void setCurrentlyActive(bool isReplay) override
	{
		currentlyActive = isReplay;
		m_frameBuffer->setCurrentlyActive(isReplay);
	}

	void switchToStoredPlayback(std::string eventName)
	{
		m_frameBuffer->switchToStoreState(eventName);
	}

	void setSeekerPosition(int position);

	int getCurrentStoreSize()
	{
		return m_frameBuffer->size();
	}

private:
	json currentFrame;
	std::shared_ptr<FrameBuffer> m_frameBuffer;
	std::shared_ptr<EventDataStore> m_eventDataStore;
};