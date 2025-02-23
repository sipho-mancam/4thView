#include "stream_data_store.hpp"
#include "views/cricket_oval_widget.hpp"

StreamDataStore::StreamDataStore()
	:currentClickedId(-1)
{
}

void StreamDataStore::dataCallback(json data)
{
	{
		std::lock_guard<std::mutex> lck(mtx);
		currentState = data;
	}
	updateCurrentState();
	Q_EMIT dataChanged(currentState);
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
