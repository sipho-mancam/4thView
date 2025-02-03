#include "types.hpp"


namespace KEvents
{
	json Event::getEventAsJson()
	{
		json result;
		result["event_id"] = eventId;
		result["source_module"] = sourceModule;
		result["event_name"] = eventName;
		result["event_data"] = eventData;
		return result;
	}
	std::string Event::serializeEvent()
	{
		json result = getEventAsJson();
		return result.dump();
	}

	void Event::__deserializeEvent(std::string& str)
	{
		json result = json::parse(str);
		eventId = result["event_id"];
		sourceModule = result["source_module"];
		eventName = result["event_name"];
		eventData = result["event_data"];

	}
	std::string Event::serializeEvent(json event)
	{
		return event.dump();
	}

	Event Event::deserializeEvent(std::string e)
	{
		Event event;
		event.__deserializeEvent(e);
		return event;
	}

	Event::Event()
		:eventId(0),
		eventType(EventType::DEFAULT)
	{

	}
	Event::Event(const Event& obj)
	{
		eventName = obj.eventName;
		eventId = obj.eventId;
		eventData == obj.eventData;
		eventType = obj.eventType;
	}

	Event::Event(Event&& obj) noexcept
	{
		eventName = obj.eventName;
		eventId = obj.eventId;
		eventData == obj.eventData;
		eventType = obj.eventType;
	}

	Event& Event::operator=(const Event& obj)
	{
		eventName = obj.eventName;
		eventId = obj.eventId;
		eventData == obj.eventData;
		eventType = obj.eventType;
		return std::ref(*this);
	}
}


