#include "ue_stream_callback.hpp"

UECStreamCallback::UECStreamCallback(json globConfig, std::string servName)
	:globalConfig(globConfig),
	serviceName(servName)
{
	int port = globalConfig[serviceName]["unrealPort"];
	std::vector<std::string> ips = globalConfig[serviceName]["unrealIp"];
	for (std::string addr : ips)
	{
		UdpSocket* udpClient = new UdpSocket(addr, port);
		udpClients.push_back(udpClient);
	}
}

void UECStreamCallback::execute(KEvents::Event e)
{
	static long long frameCount = 0;
	if (e.getEventName() == EN_STREAM_DATA_UPDATE)
	{
		json unrealEvent;
		// This converts from screen coordinates to cartesian coordinates
		json eventData = CoordinateSpaceTransform::convertFromFrameData(e.getEventData());
		unrealEvent["payload"] = eventData;
		unrealEvent["event_type"] = EventTypes::PLAYER_CONTROL;

		if (eventData.contains("distance_objects"))
		{
			//std::cout << "We have some distance objects " << std::endl;
			json distanceEvent;
			distanceEvent["payload"] = { {"control_type", "distance"}, { "distance_objects", eventData["distance_objects"] }};
			distanceEvent["event_type"] = EventTypes::SYSTEM_CONTROL;

			for (auto udpClient : udpClients)
				udpClient->sendJson(distanceEvent);
		}

		//std::cout << eventData << "\n\n" << std::endl;
		printf("\rJson payload sent to engine ... (%lld)", frameCount);

		for(auto udpClient : udpClients)
			udpClient->sendJson(unrealEvent);
		
		frameCount += 1;
	}
}

UECStreamCallback::~UECStreamCallback()
{
	for(UdpSocket* udpClient: udpClients)
		delete udpClient;
}
