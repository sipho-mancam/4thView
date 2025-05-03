#pragma once
#include "kevents.hpp"
#include <QObject>
#include <QRect>
#include <memory>

class SystemEventsSender : public QObject
{
	Q_OBJECT
public:
	SystemEventsSender(KEvents::EventsManager* eventsManager,  QObject* parent = nullptr);
	~SystemEventsSender();

	void playerPositionChanged(int trackId, QPointF itemPositionNormalized);
	void createPlayer(int trackId, QPointF itemPositionNormalized);
	void sendEvent(std::string topic, KEvents::Event e);
	void clearAllPlottedPlayers();
	
private:
	void __send_event__(std::string topic, KEvents::Event e);
private:
	KEvents::EventsManager* eventsMan;
	json config;
};