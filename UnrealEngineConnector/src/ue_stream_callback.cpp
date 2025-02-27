#include "ue_stream_callback.hpp"

UECStreamCallback::UECStreamCallback(json globConfig, std::string servName)
	:globalConfig(globConfig),
	serviceName(servName),
	udpClient(nullptr)
{
	int port = globalConfig[serviceName]["unrealPort"];
	std::string ip = globalConfig[serviceName]["unrealIp"];

	udpClient = new UdpSocket(ip, port);

}

void UECStreamCallback::execute(KEvents::Event e)
{
	if (e.getEventName() == EN_STREAM_DATA_UPDATE)
	{
		json unrealEvent;
		unrealEvent["payload"] = e.getEventData();
		unrealEvent["event_type"] = EventTypes::PLAYER_CONTROL;
		udpClient->sendJson(unrealEvent);
	}
}

UECStreamCallback::~UECStreamCallback()
{
	if(udpClient)
		delete udpClient;
}
