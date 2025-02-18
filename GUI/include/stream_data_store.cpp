#include "stream_data_store.hpp"
#include "views/cricket_oval_widget.hpp"

StreamDataStore::StreamDataStore()
{
}

void StreamDataStore::dataCallback(json data)
{
	std::cout << "Data Callback has been called ...\n";

	currentState = data;
	for (auto func : registeredUICallbacks)
	{
		func(data);
	}
}

void StreamDataStore::setCurrentClicked(int trackId)
{
	currentClickedId = trackId;
	if (!currentState.contains("tracks"))
		return;

	updateCurrentState();
}

void StreamDataStore::updateCurrentState()
{
	std::vector<json> tracks = currentState["tracks"];
	for (json track : tracks)
	{
		if (track["track_id"] == currentClickedId)
		{
			track["state"] = PlayerItemWidget::E_STATE::SELECTED;
			currentClickedTrack = track;
		}
	}
}
