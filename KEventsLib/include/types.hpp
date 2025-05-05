#pragma once

#ifdef _EXPORT
#define KEVENTS_API __declspec(dllexport)
#else
#define KEVENTS_API __declspec(dllimport)
#endif

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <nlohmann/adl_serializer.hpp>
#include <string>

#include "concurrent_queue.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <chrono>

#define EN_DATA_REQUEST "data_request"
#define EN_DATA_RESPONSE "data_response"
#define EN_STREAM_DATA "stream_data"
#define EN_STREAM_DATA_UPDATE "stream_data_update"
#define EN_STATE_MOD "state_mod"
#define EN_STATE_CAPTURE_START "state_capture_start"
#define EN_STATE_CAPTURE_STOP "state_capture_stop"
#define EN_STATE_CAPTURE_LOAD "state_capture_load" // pull the data from the stored state and load.
#define EN_STREAM_PAUSE "stream_pause"
#define EN_STREAM_RESUME "stream_resume"
#define TE_STREAM_EXT "_stream"
#define EN_SET_STREAM_MODE "set_stream_mode"
#define EN_SET_SEEKER_POSITION "set_seeker_position"
#define EN_PLAYBACK_CONTROL "playback_control"
#define EN_ERROR "error"
#define EN_FRAME_STORE_SIZE "frame_store_size"
#define EN_LOAD_STORED_STATE "load_stored_state"

#define PI_ADD_PLAYER "add_player"
#define PI_REMOVE_PLAYER "remove_player"
#define PI_UPDATE_PLAYER "update_player"
#define PI_CLEAR_ALL_PLAYERS "clear_all"
#define PI_PLACE_KICKER "place_kicker"

using json = nlohmann::json;
using namespace nlohmann::literals;

namespace KEvents
{
	extern "C" std::shared_ptr<spdlog::logger> kEventsLogger;
	/**
	* @brief
	* Forward declarations for most of the defined types in seperate files
	* 
	*/
	class CallBackBase;
	class ExecutorBase;
	class ExecutorTree;
	class EventProducer;
	class EventConsumer;
	class ThreadPool;
	class RouterBase;
	class RunnableThread;
	class DefaultExecutor;
	struct Event;

	using EventQueue = concurrent::ConcurrentQueue<Event>;
	using ExecutorBasePtr = std::shared_ptr<ExecutorBase>;
	using EventProducerPtr = std::shared_ptr<EventProducer>;
	using ThreadPoolPtr = std::shared_ptr<ThreadPool>;
	using CallBackBasePtr = std::shared_ptr<CallBackBase>;
	using RunnableThreadPtr = std::shared_ptr<RunnableThread>;
	using RouterPtr = std::shared_ptr<RouterBase>;
	using EventQueuePtr = std::shared_ptr<EventQueue>;
	using ExecutorTreePtr = std::shared_ptr<ExecutorTree>;
	using Router = RouterBase;

	typedef unsigned __int64 ulong;
	typedef unsigned int uint;

	json __load_config__();
	/**
	* @brief
	* Any module implementing the executor tree uses this model to decide how to router events to their respective executors.
	*/ 
	
	enum EventType
	{
		DEFAULT = -1, // ignored events.
		E_GUI, // GUI related events
		E_DATA_AGGREGATOR,// data aggregator related events
		E_INPUT, // input related events
		E_PROCESSOR, // processor events
		E_SPORT_EVENT_PROCESSOR, // sport_event_processor related events
		E_OUTPUT, // Output related events
		E_MISC_EVENTS // Arbitrary modules related events.
	};

	enum EPLAYER_POSITIONS {
		C_ERROR = -1,
		FIELDER = 0,
		BATMAN_A = 1,
		BATMAN,
		UMPIRE,
		BOWLER
	};
	
	
	enum STATES_DEF {
		ANNOTATION = 1,
		HIGHLIGHT = 2,
		DISTANCE = 3,
		POSITION = 4,
		PLAYER = 5
	};
	
	enum STREAM_TYPES {
		TRACKER = 0
	};

	std::string __playerPosition2Str__(EPLAYER_POSITIONS pos);
	EPLAYER_POSITIONS __str2PlayerPosition__(std::string pos);

	struct Event
	{
	public:
		long eventId; // unique ID generated for the event.
		EventType eventType; // The type of module that created this event
		std::string sourceModule; // The module name that generated this event.
		std::string eventName; // This is used by the router to understand which callback to invoke for this particular event.
		json eventData;

		Event();
		Event(const Event& obj); // Copy Constructor
		Event(Event&& obj) noexcept;
		Event& operator=(const Event& obj); // Copy Assignable
		~Event() = default;


		static Event* createEvent();
		Event* setEventName(std::string eName);
		Event* setSourceModule(std::string sourceModule);
		Event* setEventData(json data);
		Event* setEventType(EventType eType);

		long getEventID() { return eventId; }
		EventType getEventType() { return eventType; }
		std::string getSourceModuleName() { return sourceModule; }
		std::string getEventName() { return eventName; }
		json getEventData() { return eventData; }
		json getEventAsJson();

		std::string serializeEvent();
		void __deserializeEvent(std::string& str);

		static std::string serializeEvent(json  event);
		static Event deserializeEvent(std::string e);
	};
}

