#pragma once
#include "data_store_base.hpp"
#include <qobject.h>
#include "kevents.hpp"
#include <chrono>
#include <mutex>

class PlayerItemWidget;

class StreamDataStore : public QObject, public DataStoreBase
{
	Q_OBJECT

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
	void updateCurrentState();

public slots:
	void setCurrentClicked(int trackId);
	

signals:
	void dataChanged(json);
	/**
	* @brief
	* This one fires continuously for every frame with the current state object.
	*/
	void currentSelectedChangedSig(json currentTrack);
	/**
	* @brief
	* This signal will fire once only for every state change (New player selected)
	* Do not trust the changing information here, because it will be obsolete
	* Only Fixed state variables like:
	* Highlight, Position, Annotation, etc, that doesn't changed per frame
	* until the user updates.
	*/
	void selectedStateChagedSig(json currentState);
	void frameRateChangedSig(double frameRate);

private:
	std::vector<std::function<void(json)>> registeredUICallbacks;
	json currentState, currentClickedTrack;
	int currentClickedId;
	double avgFrameRate;
	std::mutex mtx;
	std::chrono::high_resolution_clock::time_point startPoint, endPoint;
};