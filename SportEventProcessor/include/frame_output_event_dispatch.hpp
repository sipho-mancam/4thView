#pragma once
#include "kevents.hpp"
#include "play_mode_manager.hpp"


class FrameOutputEventDispatch
{
public:
	FrameOutputEventDispatch(std::shared_ptr<PlayModeManager> playModeMan, KEvents::EventsManager* evMan)
		:eventsManager(evMan),
		playModeManager(playModeMan)
	{
		std::string processorName = "Processor";
		std::string guiName = "GUI";
		json globalConfig = KEvents::__load_config__();
		OutputTopic = globalConfig[processorName]["serviceTopic"];
		GuiTopic = globalConfig[guiName]["serviceTopic"];

		exit = false;
		outputWorker = new std::thread(&FrameOutputEventDispatch::dispatchOutput, this);
	}
	~FrameOutputEventDispatch()
	{
		exit = true;
		lck.notify();
		if (outputWorker && outputWorker->joinable())
		{
			outputWorker->join();
			delete outputWorker;
			KEvents::kEventsLogger->info("FrameOutputEventDispatch Stopped");
		}
	}
	/**
	* @brief
	* This method request and sends current frame as output.
	*/
	void dispatchOutput()
	{
		while (!exit)
		{
			lck.wait();

			int storeSize = playModeManager->getCurrentStoreSize();
			KEvents::Event storeSizeEvent = buildStoreSizeEvent(storeSize);
			eventsManager->sendEvent(GuiTopic, storeSizeEvent);

			// request the frame here
			json currentFrame = playModeManager->getCurrentFrame();
			if (currentFrame.empty())
			{
				currentFrame["tracks"] = std::vector<json>();
				currentFrame["frame_number"] = 0;
				currentFrame["stream_type"] = KEvents::STREAM_TYPES::TRACKER;
			}

			KEvents::Event ev = buildFrameEvent(currentFrame);
			eventsManager->sendEvent(OutputTopic, ev);


			int currentSeekerPosition = playModeManager->getCurrentSeekerPosition();
			KEvents::Event seekerEvent = buildCurrentSeekerEvent(currentSeekerPosition);
			eventsManager->sendEvent(GuiTopic, seekerEvent);
		}	
	}

	KEvents::Event buildFrameEvent(json frame)
	{
		KEvents::Event ev;
		ev.setEventName(EN_STREAM_DATA);
		ev.setEventData(frame);
		ev.setSourceModule("SportEventProcessor");
		ev.setEventType(KEvents::EventType::E_SPORT_EVENT_PROCESSOR);
		return ev;
	}

	KEvents::Event buildStoreSizeEvent(int storeSize)
	{
		KEvents::Event ev;
		ev.setEventName(EN_FRAME_STORE_SIZE);
		ev.setEventData({{"store_size", storeSize}});
		ev.setSourceModule("SportEventProcessor");
		ev.setEventType(KEvents::EventType::E_SPORT_EVENT_PROCESSOR);
		return ev;
	}

	KEvents::Event buildCurrentSeekerEvent(int currentSeekPosition)
	{
		KEvents::Event ev;
		ev.setEventName(EN_SET_SEEKER_POSITION);
		ev.setEventData({ {"seeker_position", currentSeekPosition} });
		ev.setSourceModule("SportEventProcessor");
		ev.setEventType(KEvents::EventType::E_SPORT_EVENT_PROCESSOR);
		return ev;
	}


	/**
	* @brief
	* This will be called every tick by the OutputClock
	*/
	void tick()
	{
		lck.notify();
	}

private:
	KEvents::Lock lck;
	json currentFrame;
	std::atomic<bool> exit;
	std::thread* outputWorker;
	KEvents::EventsManager* eventsManager;
	std::shared_ptr<PlayModeManager> playModeManager;
	std::string OutputTopic, GuiTopic;
};
