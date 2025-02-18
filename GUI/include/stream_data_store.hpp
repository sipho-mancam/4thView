#pragma once
#include "data_store_base.hpp"

class PlayerItemWidget;

class StreamDataStore : public DataStoreBase
{
public:
	StreamDataStore();

	// We'll register this function with the stream callback, so tha
	void dataCallback(json data);

	template<typename T>
	void registerUICallback(T func)
	{
		registeredUICallbacks.emplace_back(func);
	}

	json getCurrentState() { return currentState; }
	json getCurrentClicked() { return currentClickedTrack; }
	void setCurrentClicked(int trackId);
	void updateCurrentState();


private:
	std::vector<std::function<void(json)>> registeredUICallbacks;
	json currentState, currentClickedTrack;
	int currentClickedId;
};