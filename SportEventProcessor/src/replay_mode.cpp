#include "replay_mode.hpp"


ReplayMode::ReplayMode()
{
	m_frameBuffer = std::make_shared<FrameBuffer>();
}

void ReplayMode::setSeekerPosition(int position)
{
	m_frameBuffer->setCurrentFrameIndex(position);
}
