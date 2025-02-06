//// KEventsLib.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//#include <iostream>
//#include "kevents.hpp"
//#include <conio.h>
//
//
//#include "testSuite/callbacks_test.hpp"
//
//int main()
//{
//	{
//		std::cout << "System running ... " << std::endl;
//		KEvents::EventsManager eventsManager("events", "test_group");
//
//
//		std::shared_ptr<KEvents::TestCallback>	testCb = std::make_shared<KEvents::TestCallback>();
//		std::shared_ptr<KEvents::TestCallback2> testCb2 = KEvents::createCallback<KEvents::TestCallback2>();
//		std::shared_ptr<KEvents::TestCallback3> testCb3 = KEvents::createCallback<KEvents::TestCallback3>("hello_friend");
//		std::shared_ptr<KEvents::TestCallback4> testCb4 = KEvents::createCallback<KEvents::TestCallback4>("hello world", "how are you");
//
//		eventsManager.registerCallback("test_event", testCb);
//		eventsManager.registerCallback("test_event", testCb2);
//		eventsManager.registerCallback("purchase", testCb);
//		eventsManager.registerCallback("shipment_sent", testCb2);
//		eventsManager.registerCallback("item_added", testCb3);
//		eventsManager.registerCallback("shipment_sent", testCb4);
//
//		// Register all your callbacks here ... 
//		eventsManager.startEventLoop();
//		// continue with your other business here ...
//		_getch();
//	}
//	
//	std::cout << "System shutdown complete ..." << std::endl;
//
//	return 0;
//}
