#include <iostream>
#include "kevents.hpp"
#include <conio.h>



int main()
{
	json config = KEvents::__load_config__();
	json moduleConfig = config["InputManager"];

	std::string serviceTopic = moduleConfig["serviceTopic"];
	std::string serviceName = moduleConfig["serviceName"];

	KEvents::EventsManager eventsManager(serviceTopic, serviceName);

	eventsManager.startEventLoop();

	_getch();

	return 0;
	
}
