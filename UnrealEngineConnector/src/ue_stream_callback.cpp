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
	if (e.getEventName() == EN_STREAM_DATA_UPDATE)
	{
		json unrealEvent;
		// This converts from screen coordinates to cartesian coordinates
		json eventData = CoordinateSpaceTransform::convertFromFrameData(e.getEventData());
		unrealEvent["payload"] = eventData;
		unrealEvent["event_type"] = EventTypes::PLAYER_CONTROL;

		for(auto udpClient : udpClients)
			udpClient->sendJson(unrealEvent);
	}
}

UECStreamCallback::~UECStreamCallback()
{
	for(UdpSocket* udpClient: udpClients)
		delete udpClient;
}
