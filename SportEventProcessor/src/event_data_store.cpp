#include "event_data_store.hpp"
#include <iomanip>
#include <ctime>

std::string getTodayDate() {
	// Get the current time
	std::time_t now = std::time(nullptr);
	
	std::tm localTime;
	localtime_s(&localTime, &now);

	// Format the date as dd-mm-yyyy
	char buffer[11]; // Buffer to hold the formatted date (10 characters + null terminator)
	std::strftime(buffer, sizeof(buffer), "%d-%m-%Y", &localTime);

	return std::string(buffer);
}


EventDataStore::EventDataStore(std::string rootWrDir)
	:rootWriteDir(rootWrDir),
	currentPtr(0),
	eventOpen(false)
{
	std::string root4thViewDir = "4th_View\\";
	if (!std::filesystem::is_directory(rootWriteDir + root4thViewDir))
	{
		std::filesystem::create_directory(rootWriteDir + root4thViewDir);
		KEvents::kEventsLogger->info("Created root data store directory at: {}", rootWriteDir + root4thViewDir);
	}

	std::string currentStoreName = getTodayDate();
	
	currentStore = rootWriteDir + root4thViewDir + currentStoreName + "\\";
	if (!std::filesystem::is_directory(currentStore))
	{
		if (std::filesystem::create_directory(currentStore))
		{
			KEvents::kEventsLogger->info("Created current event store at: {}", currentStore);
		}
		else {
			KEvents::kEventsLogger->error("Failed to create event store");
			throw std::exception("Failed to create event store");
		}
	}
}

void EventDataStore::startCapture(json eventData)
{
	stopCapture();
	if (!eventOpen)
	{
		currentEvent = eventData;
		std::string sportEventName = eventData["sport_event_name"];
		sportEventName += "\\";
		currentEvent["event_dir"] = currentStore  + sportEventName;

		if (std::filesystem::create_directory(currentEvent["event_dir"]))
		{
			std::string d = currentEvent["event_dir"];
			KEvents::kEventsLogger->info("Created current event at: {}", d);
		}
		eventOpen = true;
	}
}

void EventDataStore::stopCapture()
{
	eventOpen = false;
	currentPtr = 0;
}

void EventDataStore::capture(json frameData)
{
	if (!eventOpen)
	{
		//KEvents::kEventsLogger->warn("Event Store not open at");
		return;
	}

	std::string frame_data = frameData.dump();
	std::string currentEventDir = currentEvent["event_dir"];
	
	std::string fileName = __get_file_name__(currentPtr, -1);
	std::ofstream writeFile(fileName);
	writeFile.write(frame_data.c_str(), frame_data.size());
}

void EventDataStore::setCurrentEvent(json eventData)
{
	currentEvent = eventData;
	std::string sportEventName = eventData["sport_event_name"];
	currentEvent["event_dir"] = currentStore + sportEventName + "\\";
}

json EventDataStore::getEventsList()
{
	return json();
}

void EventDataStore::seekCurrentEvent(int ptr)
{

}

std::vector<json> EventDataStore::loadEvent(std::string eventName)
{
	/**
	* 1. Check if the event directory exists
	* 2. if the directory exists, load the event frames, sort them and return the list of frames
	* 3. if directory doesn't exist, return empty vector
	***/
	std::string eventDir = currentStore + eventName + "\\";
	std::vector<std::filesystem::path> eventFrames;
	if (std::filesystem::is_directory(eventDir))
	{
		for (const auto& entry : std::filesystem::directory_iterator(eventDir))
		{
			if (entry.path().extension() == ".json")
			{
				eventFrames.push_back(entry.path());
			}
		}
		std::sort(eventFrames.begin(), eventFrames.end());

		std::vector<json> frames;
		for (const auto& frame : eventFrames)
		{
			std::ifstream readFile(frame);
			json frameData;
			readFile >> frameData;
			frames.push_back(frameData);
		}
		return frames;
	}
	return std::vector<json>();
}

std::string EventDataStore::__get_file_name__(uint16_t cptr, int ballN)
{
	std::string sportEventName = currentEvent["sport_event_name"];
	std::string fileName = currentStore + sportEventName + "\\frame_" + "_" + std::to_string(currentPtr) + ".json";
	currentPtr += 1;
	return fileName;
}
