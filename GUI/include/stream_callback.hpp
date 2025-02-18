#pragma once
#include "kevents.hpp"


class StreamCallback : public KEvents::CallBackBase
{
public:
	StreamCallback(json globConfig, std::string servName);

	virtual void execute(KEvents::Event e) override;
	
	template<typename F>
	void registerDataCallback(F _cb)
	{
		registeredDataCallbacks.emplace_back(_cb);
	}
private:
	json globalConfig;
	std::string serviceName;
	std::vector<std::function<void(json)>> registeredDataCallbacks;

};