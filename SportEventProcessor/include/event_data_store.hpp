#pragma once
#include <filesystem>
#include "kevents.hpp"

class EventDataStore
{
public:
	EventDataStore(std::string rootWrDir);

	void startCapture(json eventData);
	void stopCapture();
	void capture(json frameData);

	void setCurrentEvent(json eventData);
	json getEventsList();
	void seekCurrentEvent(int ptr);
	std::vector<json> loadEvent(std::string eventName);

private:
	std::string __get_file_name__(uint16_t cptr, int ballN);
	bool eventOpen;
	std::string rootWriteDir;
	json currentEvent;
	std::string currentStore;
	uint16_t currentPtr;

};

