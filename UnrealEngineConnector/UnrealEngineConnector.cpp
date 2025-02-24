#include <iostream>
#include "kevents.hpp"
#include "ue_stream_callback.hpp"

int main()
{
    json globalConfig = KEvents::__load_config__();
    json systemSettings = globalConfig["systemSettings"];
    json moduleConfig = globalConfig["UnrealEngineConnector"];
    std::string serviceName, serviceTopic;

    serviceName = moduleConfig["serviceName"];
    serviceTopic = moduleConfig["serviceTopic"];

    KEvents::EventsManager eventManager(serviceTopic, serviceName);
   
    std::shared_ptr<UECStreamCallback> ueStreamCb = KEvents::createCallback<UECStreamCallback>(globalConfig, serviceName);
   
    eventManager.registerCallback(EN_STREAM_DATA_UPDATE, ueStreamCb);
  
    eventManager.startEventLoop(true);

    return EXIT_SUCCESS;
}