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
	json eventData, playerData, payload;

	playerData["track_id"] = trackId;
	playerData["coordinates"] = std::vector<double>({ itemPositionNormalized.x(), itemPositionNormalized.y() });

	payload["instruction"] = PI_UPDATE_PLAYER;
	payload["data"] = playerData;
	eventData["state_def"] = KEvents::STATES_DEF::PLAYER;
	
	eventData["data"] = payload;

	std::cout << "Player Position Changed: " << eventData << std::endl;
	e.setEventData(eventData);
	e.setSourceModule("gui");
	e.setEventName(EN_STATE_MOD);
	e.setEventType(KEvents::E_GUI);
	
	std::string topic = config["DataAggregator"]["serviceTopic"];
	__send_event__(topic, e);
}

void SystemEventsSender::createPlayer(int trackId, QPointF itemPositionNormalized)
{
	KEvents::Event e;
	json eventData, playerData, payload;
	playerData["track_id"] = trackId;
	playerData["coordinates"] = std::vector<double>({0.5, 0.5});
	playerData["team"] = "default";
	playerData["kit_color"] = std::vector<int>({ 0, 0, 0 });
	playerData["state"] = 0;
	playerData["player_type"] = PLAYER_TYPE::PLOTTED;

	payload["instruction"] = PI_ADD_PLAYER;
	payload["data"] = playerData;

	eventData["state_def"] = KEvents::STATES_DEF::PLAYER;
	eventData["data"] = payload;
	
	std::cout << "Player create event generated: " << eventData << std::endl;
	e.setEventData(eventData);
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

void SystemEventsSender::clearAllPlottedPlayers()
{
	KEvents::Event e;
	json eventData, payload;
	eventData["state_def"] = KEvents::STATES_DEF::PLAYER;
	payload["instruction"] = PI_CLEAR_ALL_PLAYERS;
	eventData["data"] = payload;

	std::cout << "Generated clear all event: " << eventData << std::endl;

	e.setEventData(eventData);
	e.setSourceModule("gui");
	e.setEventName(EN_STATE_MOD);
	e.setEventType(KEvents::E_GUI);
	std::string topic = config["DataAggregator"]["serviceTopic"];
	__send_event__(topic, e);
}

void SystemEventsSender::__send_event__(std::string topic, KEvents::Event e)
{
	eventsMan->sendEvent(topic, e);
	std::cout << "Event Sent to topic: " << topic << std::endl;
}
