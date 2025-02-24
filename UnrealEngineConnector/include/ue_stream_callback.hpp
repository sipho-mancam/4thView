#pragma once
#include "kevents.hpp"
#include "ue_udp_client.hpp"

class UECStreamCallback : public KEvents::CallBackBase
{
	enum EventTypes {
		PLAYER_CONTROL = 0X01,
		SYSTEM_CONTROL = 0X02,
		LEVEL_CONTROL = 0X03
	};
public:
	UECStreamCallback(json globalConfig, std::string serviceName);

	virtual void execute(KEvents::Event e);

	~UECStreamCallback();
private:
	json globalConfig;
	std::string serviceName;
	UdpSocket* udpClient;
};