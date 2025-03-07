#include <iostream>
#include "kevents.hpp"
#include "tracker_consumer.hpp"
#include "output_callback.hpp"

int main()
{
    json globalConfig = KEvents::__load_config__();
    json systemSettings = globalConfig["systemSettings"];
    json moduleConfig = globalConfig["TrackingCoreConnector"];
    std::string serviceName, serviceTopic;

    serviceName = moduleConfig["serviceName"];
    serviceTopic = moduleConfig["serviceTopic"];

    std::shared_ptr<OutputCallback> outputCb = KEvents::createCallback<OutputCallback>(globalConfig, serviceName);

    KEvents::kEventsLogger->info("Starting {}", serviceName);
    KEvents::EventsManager eventManager(serviceTopic, serviceName);
    /**
    * register callbacks here.
    */
    eventManager.registerCallback(EN_STREAM_DATA, outputCb);
    eventManager.startEventLoop();

    std::atomic<bool> existFlag = false;
    TrackerConsumer trackConsumer(globalConfig, serviceName);

    while (!existFlag)
    {
        if (trackConsumer.waitForMessage())
        {
            json message = trackConsumer.getCurrentMessage();
            if (message.empty())
                continue;

            KEvents::Event e;
            e.setEventType(KEvents::EventType::E_INPUT);
            e.setEventName(EN_STREAM_DATA);
            e.setSourceModule(serviceName);
            message["stream_type"] = KEvents::STREAM_TYPES::TRACKER;
            e.setEventData(message);
            // This is a cheat, hacking the callback system to execute events on our own.
            // But should be common for data generators
            outputCb->execute(e);
            
        }
    }
    return 0;
}