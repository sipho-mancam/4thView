#include "tracker_consumer.hpp"

TrackerConsumer::TrackerConsumer(json globConfig, std::string servName)
	:globalConfig(globConfig),
	serviceName(servName),
	messageStatus(false)
{
	json& moduleConfig = globalConfig["TrackingCoreConnector"];
	std::string broker = moduleConfig["kafkaBroker"];
	kConsumer = KEvents::buildConsumer(broker, serviceName);
	KEvents::kEventsLogger->info("Connecting to tracking core kafka at address: {}", broker);
	
	std::string trackerTopic = moduleConfig["trackerAddress"];

	kConsumer->subscribe(std::vector<std::string>({ trackerTopic }));
}

json TrackerConsumer::waitForMessage()
{
	messageStatus = false;
	RdKafka::Message* message = kConsumer->consume(20);
	if (message->err() != RdKafka::ERR_NO_ERROR)
	{
		delete message;
		return json();
	}	
	currentMessage = std::string(static_cast<char*>(message->payload()), message->len());
	messageStatus = true;
	delete message;

	try {
		json msg = json::parse(currentMessage);
		return msg;
	}
	catch (json::parse_error& pe) {
		KEvents::kEventsLogger->error("Failed to parse json incoming message from tracker...");
		KEvents::kEventsLogger->error("{}", pe.what());
		return json();
	}

	return json();
}

json TrackerConsumer::getCurrentMessage()
{
	if (messageStatus)
	{
		try {
			json msg = json::parse(currentMessage);
			return msg;
		}
		catch (json::parse_error& pe) {
			KEvents::kEventsLogger->error( "Failed to parse json incoming message from tracker...");
			KEvents::kEventsLogger->error("{}", pe.what());
			return json();
		}
	}
	return json();
}
