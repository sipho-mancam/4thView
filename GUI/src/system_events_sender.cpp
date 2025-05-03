#include "system_events_sender.hpp"
#include "views/cricket_oval_widget.hpp"

SystemEventsSender::SystemEventsSender(KEvents::EventsManager* eventsManager, QObject* parent)
	:QObject(parent),
	eventsMan(eventsManager)
{
	config = KEvents::__load_config__();
}

SystemEventsSender::~SystemEventsSender()
{
}

void SystemEventsSender::playerPositionChanged(int trackId, QPointF itemPositionNormalized)
{
	KEvents::Event e;
	json playerData;
	playerData["track_id"] = trackId;
	playerData["coordinates"] = std::vector<double>({itemPositionNormalized.x(), itemPositionNormalized.y()});
	playerData["playerType"] = PLAYER_TYPE::PLOTTED;
	playerData["team"] = "default";

	e.setEventData(playerData);
	e.setSourceModule("gui");
	e.setEventName(EN_STATE_MOD);
	e.setEventType(KEvents::E_GUI);
	
	std::string topic = config["DataAggregator"]["serviceTopic"];
	__send_event__(topic, e);
}

void SystemEventsSender::sendEvent(std::string topic, KEvents::Event e)
{
	__send_event__(topic, e);
}

void SystemEventsSender::__send_event__(std::string topic, KEvents::Event e)
{
	eventsMan->sendEvent(topic, e);
}
