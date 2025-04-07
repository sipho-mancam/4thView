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
	std::string inputManagerName = "InputManager";
	std::string inputManagerTopic = globalConfig[inputManagerName]["serviceTopic"];

    std::atomic<bool> existFlag = false;
    TrackerConsumer trackConsumer(globalConfig, serviceName);

    std::chrono::time_point<std::chrono::steady_clock> execTimeStart, execTimeEnd;

    long long frameCounter = 0;
    while (!existFlag)
    {
		json message = trackConsumer.waitForMessage();
        if (!message.empty())
        {
            long long elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(execTimeEnd - execTimeStart).count();
            if (elapsedTime >= 1)
            {
                execTimeStart = std::chrono::steady_clock::now();
                double frameRate = (frameCounter * 1.0) / (elapsedTime * 1.0);
                KEvents::kEventsLogger->info("Frame rate:{} fps ... elapsed Time:{} seconds.", frameRate, elapsedTime);
                frameCounter = 0;
            }

            KEvents::Event e;
            e.setEventType(KEvents::EventType::E_INPUT);
            e.setEventName(EN_STREAM_DATA);
            e.setSourceModule(serviceName);
            message["stream_type"] = KEvents::STREAM_TYPES::TRACKER;
            e.setEventData(message);
            // This is a cheat, hacking the callback system to execute events on our own.
            // But should be common for data generators
            //outputCb->execute(e);
			eventManager.sendEvent(inputManagerTopic, e);
			execTimeEnd = std::chrono::steady_clock::now();
			frameCounter += 1; 
        }
    }
    return 0;
}