#include "types.hpp"
#include <Bits.h>

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
				"systemSettings":{
					"kafka":{
						"broker":"127.0.0.1:9092"
					},
					"registeredModules":[
						"InputManager",
						"Processor",
						"DataAggregator",
						"OutputManager",
						"SportEventProcessor",
						"TrackingCoreConnector",
						"GUI"
					]
				},
    
				"kEventslib":{
					"poolSize": 3,
					"kafka":{
						"broker":"127.0.0.1:9092"
					}
				},

				"GUI":{
					"serviceName":"gui",
					"serviceTopic":"gui_mod"
				},

				"Processor":{
					"serviceName":"processor",
					"serviceTopic":"processor_mod"
				},
				"DataAggregator":{
					"serviceName":"data_aggregator",
					"serviceTopic":"data_aggregator_mod"
				},
				"OutputManager":{
					"serviceName":"output_manager",
					"serviceTopic":"output_manager_mod",
					"connectors_list":[
						"unrealEngine",
						"vizEngine"
					]
				},
				"UnrealEngineConnector":{
					"serviceName":"unreal_engine_con",
					"serviceTopic": "unreal_engine_mod"
				},
				"VizEngineConnector":{
					"serviceName":"viz_engine_con",
					"serviceTopic": "viz_engine_mod"
				},
				"SportEventProcessor":{
					"serviceName":"sport_event_processor",
					"serviceTopic": "sport_event_processor_mod"
				},
				"TrackingCoreConnector":{
					"serviceName":"tracking_core_connector",
					"serviceTopic": "tracking_core_connector_mod",
					"trackerAddress":"system-output"
				},
				"InputManager":{
					"serviceName":"input_manager",
					"serviceTopic":"input_manager_mod",
					"connectors_list":[
						"plotter",
						"db_query_processor",
						"file_sys",
						"st_data"
					],
					"connectors":{
						"plotter":{
							"address":"plotter_con",
							"description":"Generates input data from the field position plotter GUI"
						},
						"db_query_processor":{
							"address":"db_query_processor_con",
							"description":"facilitates communications between database and the clients, and implements the query generator"
						},
						"file_sys":{
							"address":"file_sys_con",
							"description":"Handles file system data requests, abstracts the underlying OS file system from the system."
						},
						"st_data":{
							"address":"st_data_con",
							"description": "Receives live data from the seb tracker and feeds it to the system for processing and view."
						}
					}
				}
			}
			)");
			// dump the default config and return
			std::string dstPath = "C:/ProgramData/Player Tracking Software/4th_view_config.json";
			std::string confStr = defaultConfig.dump();
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


