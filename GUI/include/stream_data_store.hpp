#pragma once
#include "data_store_base.hpp"
#include <qobject.h>
#include "kevents.hpp"
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
	void currentSelectedChangedSig(json currentTrack);

private:
	std::vector<std::function<void(json)>> registeredUICallbacks;
	json currentState, currentClickedTrack;
	int currentClickedId;
	std::mutex mtx;
};