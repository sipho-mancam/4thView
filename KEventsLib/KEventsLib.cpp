// KEventsLib.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include "kevents.hpp"
#include <conio.h>


int main()
{
	{
		std::cout << "System running ... " << std::endl;
		KEvents::EventsManager eventsManager("events");


		// Register all your callbacks here ... 
		eventsManager.startEventLoop();
		// continue with your other business here ...
		_getch();
	}
	
	std::cout << "System shutdown complete ..." << std::endl;

	return 0;
}
