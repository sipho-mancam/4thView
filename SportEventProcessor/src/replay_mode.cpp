#include "replay_mode.hpp"


ReplayMode::ReplayMode(std::shared_ptr<EventDataStore> evDs)
	: m_eventDataStore(evDs)
{
	m_frameBuffer = std::make_shared<FrameBuffer>(m_eventDataStore);
}

void ReplayMode::setSeekerPosition(int position)
{
	m_frameBuffer->setCurrentFrameIndex(position);
}
