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
		std::string inning = eventData["inning"];
		inning += "\\";
		if (!std::filesystem::is_directory(currentStore + inning))
		{
			if (std::filesystem::create_directory(currentStore + inning)){}
		}

		std::string sportEventName = eventData["sport_event_name"];
		sportEventName += "\\";
		currentEvent["event_dir"] = currentStore + inning + sportEventName;

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
	int currentBall = currentEvent["ball_number"];
	std::string fileName = __get_file_name__(currentPtr, currentBall);
	std::ofstream writeFile(fileName);
	writeFile.write(frame_data.c_str(), frame_data.size());
}

void EventDataStore::setCurrentEvent(json eventData)
{
	currentEvent = eventData;
	std::string inning = eventData["inning"];
	inning += "\\";
	std::string sportEventName = eventData["sport_event_name"];
	currentEvent["event_dir"] = currentStore + inning + sportEventName;
}

json EventDataStore::getEventsList()
{
	return json();
}

void EventDataStore::seekCurrentEvent(int ptr)
{

}

std::string EventDataStore::__get_file_name__(uint16_t cptr, int ballN)
{
	std::string inning = currentEvent["inning"];
	inning += "\\";
	std::string sportEventName = currentEvent["sport_event_name"];
	std::string fileName = currentStore + inning + sportEventName + "\\frame_" + std::to_string(ballN) + "_" + std::to_string(currentPtr) + ".json";
	currentPtr += 1;
	return fileName;
}
