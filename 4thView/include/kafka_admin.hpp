#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <librdkafka/rdkafkacpp.h>
#include "kevents.hpp"

class KafkaTopicCreator {
public:
    KafkaTopicCreator(const std::string& broker) : broker_(broker) {
        // Create Kafka configuration
        prod = std::make_shared<KEvents::EventProducer>(broker);
    }

    ~KafkaTopicCreator() {
       
    }

    void createTopics(const std::vector<std::string>& topics) {
        for (std::string topic : topics)
        {
            // sending the first message will initialize the topic
            KEvents::Event initEvent;
            initEvent.setSourceModule("SystemStartupModule");
            initEvent.setEventType(KEvents::DEFAULT);
            initEvent.setEventName("start_up_event");
            prod->sendMessage(topic, initEvent);
            prod->sendMessage(topic+TE_STREAM_EXT, initEvent);
       }
    }

private:
    std::string broker_;
    std::shared_ptr<KEvents::EventProducer> prod;
};
