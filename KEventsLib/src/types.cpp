#include "types.hpp"
#include <Bits.h>


namespace KEvents
{
	std::shared_ptr<spdlog::logger> kEventsLogger = spdlog::stdout_color_mt("KEventsLib Logger");
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
			kEventsLogger->error("Parse error at byte: {} " ,ex.byte);
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
			  "DataAggregator": {
				"serviceName": "data_aggregator",
				"serviceTopic": "data_aggregator_mod"
			  },
			  "GUI": 
			  { 
				"serviceName": "gui", 
				"serviceTopic": "gui_mod" 
			  },
			  "InputManager": {
				"connectors": {
				  "db_query_processor": {
					"address": "db_query_processor_con",
					"description": "facilitates communications between database and the clients, and implements the query generator"
				  },
				  "file_sys": {
					"address": "file_sys_con",
					"description": "Handles file system data requests, abstracts the underlying OS file system from the system."
				  },
				  "plotter": {
					"address": "plotter_con",
					"description": "Generates input data from the field position plotter GUI"
				  },
				  "st_data": {
					"address": "st_data_con",
					"description": "Receives live data from the seb tracker and feeds it to the system for processing and view."
				  }
				},
				"connectors_list": ["plotter", "db_query_processor", "file_sys", "st_data"],
				"serviceName": "input_manager",
				"serviceTopic": "input_manager_mod"
			  },
			  "OutputManager": {
				"connectors_list": ["unrealEngine", "vizEngine"],
				"serviceName": "output_manager",
				"serviceTopic": "output_manager_mod"
			  },
			  "Processor": { "serviceName": "processor", "serviceTopic": "processor_mod" },
			  "SportEventProcessor": {
				"serviceName": "sport_event_processor",
				"serviceTopic":"sport_event_processor_mod",
				"clock_tick_interval_ms":100,
				"buffer_length":3600,
				"persistent_store_dir":"C:\\ProgramData\\Player Tracking Software\\4th_view_frame_store\\"
			  },
			  "TrackingCoreConnector": {
				"kafkaBroker": "10.0.100.96:9092",
				"serviceName": "tracking_core_connector",
				"serviceTopic": "tracking_core_connector_mod",
				"trackerAddress": "system-output"
			  },
			  "UnrealEngineConnector": {
				"serviceName": "UnrealEngineConnector",
				"serviceTopic": "unreal_engine_mod",
				"unrealIp": ["10.0.100.200"],
				"unrealPort": 6000
			  },
			  "VizEngineConnector": {
				"serviceName": "viz_engine_con",
				"serviceTopic": "viz_engine_mod"
			  },
			  "kEventslib": { "kafka": { "broker": "127.0.0.1:9092" }, "poolSize": 6 },
			  "systemSettings": {
				"kafka": { "broker": "127.0.0.1:9092" },
				"frameRate": 10.6,
				"fieldDimensions":{
				  "width":110,
				  "height":130
				},
				"registeredModules": [
				  "TrackingCoreConnector",
				  "InputManager",
				  "SportEventProcessor",
				  "Processor",
				  "DataAggregator",
				  "OutputManager",
				  "UnrealEngineConnector"
				]
			  }
			}
			)");
			// dump the default config and return
			std::string dstPath = "C:/ProgramData/Player Tracking Software/4th_view_config.json";
			std::string confStr = defaultConfig.dump();

			if (!std::filesystem::create_directory("C:/ProgramData/Player Tracking Software/"))
			{
				kEventsLogger->error("Failed to create config directory at: {}", "C:/ProgramData/Player Tracking Software/");
			}

			std::ofstream file(dstPath);
			file.write(const_cast<char*>(confStr.c_str()), confStr.size());
			file.close();
			return defaultConfig;
		}
	}

	std::string __playerPosition2Str__(EPLAYER_POSITIONS pos)
	{
		if (pos == EPLAYER_POSITIONS::BATMAN)
		{
			return "batter";
		}
		else if (pos == EPLAYER_POSITIONS::BATMAN_A)
		{
			return "batter facing";
		}
		else if (pos == EPLAYER_POSITIONS::BOWLER)
		{
			return "bowler";
		}
		else if (pos == EPLAYER_POSITIONS::FIELDER)
		{
			return "fielder";
		}
		else if (pos == EPLAYER_POSITIONS::UMPIRE)
		{
			return "umpire";
		}

		return "";
	}

	EPLAYER_POSITIONS __str2PlayerPosition__(std::string pos)
	{
		std::string lower_(pos);
		std::transform(pos.begin(), pos.end(), lower_.begin(), ::tolower);
		if (lower_ == "fielder")
		{
			return EPLAYER_POSITIONS::FIELDER;
		}
		else if (lower_ == "bowler")
		{
			return EPLAYER_POSITIONS::BOWLER;
		}
		else if (lower_ == "batter")
		{
			return EPLAYER_POSITIONS::BATMAN;
		}
		else if (lower_ == "umpire")
		{
			return EPLAYER_POSITIONS::UMPIRE;
		}
		else if (lower_ == "batter facing")
		{
			return EPLAYER_POSITIONS::BATMAN_A;
		}

		return EPLAYER_POSITIONS::C_ERROR;
	}
}


