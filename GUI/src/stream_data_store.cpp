#include "stream_data_store.hpp"
#include "views/cricket_oval_widget.hpp"

StreamDataStore::StreamDataStore()
	:currentClickedId(-1),
	avgFrameRate(0)
{
}

void StreamDataStore::dataCallback(json data)
{
	static int fCount = 0;
	endPoint = std::chrono::high_resolution_clock::now();
	long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endPoint - startPoint).count();

	double currentFrameRate = 1000.0 / elapsedTime;
	avgFrameRate += currentFrameRate;
	if (fCount == 11)
	{
		avgFrameRate = round(avgFrameRate / fCount);
		Q_EMIT frameRateChangedSig(avgFrameRate);
		fCount = 0;
		avgFrameRate = 0;
	}
	else {
		fCount++;
	}
	

	{
		std::lock_guard<std::mutex> lck(mtx);
		currentState = data;
	}
	updateCurrentState();
	Q_EMIT dataChanged(currentState);
	
	// Measure the elapsed time since last call.
	startPoint = std::chrono::high_resolution_clock::now();
}

void StreamDataStore::setCurrentClicked(int trackId)
{
	currentClickedId = trackId;
	if (!currentState.contains("tracks"))
		return;
	updateCurrentState();
	Q_EMIT selectedStateChagedSig(currentClickedTrack);
}

void StreamDataStore::updateCurrentState()
{
	std::lock_guard<std::mutex> lck(mtx);
	std::vector<json> tracks = currentState["tracks"];
	for (json& track : tracks)
	{
		if (track["track_id"] == currentClickedId)
		{
			track["state"] = PlayerItemWidget::E_STATE::SELECTED;
			currentClickedTrack = track;
		}
	}
	currentState["tracks"] = tracks;
	if(!currentClickedTrack.empty())
		Q_EMIT currentSelectedChangedSig(currentClickedTrack);
}
