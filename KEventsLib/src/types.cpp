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
		json result;
		try
		{
			result = json::parse(str);
		}
		catch (json::parse_error& ex)
		{
			std::cerr << "Parse error at byte: " << ex.byte << std::endl;
			return;
		}
		
		// check for existence of keys before trying anything
		if (!result.contains("event_id") ||
			!result.contains("source_module") ||
			!result.contains("event_name") ||
			!result.contains("event_data")
		)
			return ;

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
		eventData = obj.eventData;
		eventType = obj.eventType;
	}

	Event::Event(Event&& obj) noexcept
	{
		eventName = obj.eventName;
		eventId = obj.eventId;
		eventData = obj.eventData;
		eventType = obj.eventType;
	}

	Event& Event::operator=(const Event& obj)
	{
		eventName = obj.eventName;
		eventId = obj.eventId;
		eventData = obj.eventData;
		eventType = obj.eventType;
		return std::ref(*this);
	}
	Event* Event::createEvent()
	{
		Event* e = new Event();
		e->
		setEventType(EventType::DEFAULT)->
		setEventName("")->
		setSourceModule("")->
		setEventData(json());
		return e;
	}
	Event* Event::setEventName(std::string eName)
	{
		eventName = eName;
		return this;
	}

	Event* Event::setSourceModule(std::string sModule)
	{
		sourceModule = sModule;
		return this;
	}

	Event* Event::setEventData(json data)
	{
		eventData = data;
		return this;
	}

	Event* Event::setEventType(EventType eType)
	{
		eventType = eType;
		return this;
	}

	json __load_config__()
	{
		std::string path = "C:/ProgramData/Player Tracking Software/4th_view_config.json";
		if (std::filesystem::exists(path))
		{
			std::ifstream file(path);
			try {
				json result = json::parse(file);
				return result;
			}
			catch (json::parse_error& ex) {
				goto DefaultConfig;
			}
			
		}
		else {
			DefaultConfig:
			json defaultConfig = json::parse(R"(
			{
				"kEventslib":{
					"poolSize": 3,
					"kafka":{
						"broker":"127.0.0.1:9092"
					}
				}
			})");
			// dump the default config and return
			std::string dstPath = "C:/ProgramData/Player Tracking Software/4th_view_config.json";
			std::string confStr = defaultConfig.dump();
			std::ofstream file(dstPath);
			file.write(const_cast<char*>(confStr.c_str()), confStr.size());
			file.close();
			return defaultConfig;
		}
	}
}


