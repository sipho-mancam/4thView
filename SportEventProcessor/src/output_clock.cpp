#include "output_clock.hpp"

OutputClock::OutputClock(std::shared_ptr<FrameOutputEventDispatch> fo)
	: isInternalClockSource(false), 
	isExternalAlive(true), 
	exit(false),
	internalClockFrequency(0), 
	maxClockWaitCount(0),
	frameOutputEventDispatch(fo)
{
	config = KEvents::__load_config__();
	moduleName = "SportEventProcessor";
	tickInterval = config[moduleName]["clock_tick_interval_ms"];
	clockWorker = new std::thread(&OutputClock::__run_internalClock, this);
	mStartPoint = std::chrono::high_resolution_clock::now();
}

OutputClock::~OutputClock()
{
	if (clockWorker && clockWorker->joinable())
	{
		exit = true;
		clockWorker->join();
		delete clockWorker;
		KEvents::kEventsLogger->info("Output Clock Stopped");
	}
}


void OutputClock::externalFrameUpdate()
{
	isExternalAlive = true;
	std::lock_guard<std::mutex> lock(clockMutex);
	maxClockWaitCount = 0;
	// fire the output event...
	__dispatchOutputEvent();
}

void OutputClock::__run_internalClock()
{
	if(tickInterval < INTERMEDIATE_WAKES)
		tickInterval = INTERMEDIATE_WAKES;

	int sleepInterval = tickInterval / INTERMEDIATE_WAKES;
	startPoint = std::chrono::high_resolution_clock::now();
	while (!exit)
	{
		currentPoint = std::chrono::high_resolution_clock::now();
		while ((currentPoint - startPoint) < std::chrono::milliseconds(tickInterval))
		{
			currentPoint = std::chrono::high_resolution_clock::now();
			// check how far is the clock from the tick point
			auto diff = std::chrono::duration_cast<std::chrono::microseconds>(
				std::chrono::milliseconds(tickInterval) - 
				(currentPoint - startPoint)
			);
	
			if (diff.count() > 1000)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(sleepInterval));
				continue;
			}
			while (diff.count() > 2000)
			{
				currentPoint = std::chrono::high_resolution_clock::now();
				diff = std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::milliseconds(tickInterval) - 
					(currentPoint - startPoint)
				);
			}
			break;
		}
		auto debugDiff = startPoint;
		startPoint = std::chrono::high_resolution_clock::now();
		// dispatch the event here ... 
		// tick here ...
		if (clockMutex.try_lock())
		{
			maxClockWaitCount++;
			clockMutex.unlock();
		}
		__dispatchOutputEvent();
	}
}

void OutputClock::__dispatchOutputEvent()
{
	static long frameCount = 0;
	mEndPoint = std::chrono::high_resolution_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(mEndPoint - mStartPoint).count() >= 1)
	{
		KEvents::kEventsLogger->info("Output frame rate: {} fps", frameCount);
		mStartPoint = std::chrono::high_resolution_clock::now();
		frameCount = 0;
	}
	
	
	if (maxClockWaitCount == 0 || maxClockWaitCount >= MAX_EXTERNAL_TICK_WAIT_COUNT || !isExternalAlive)
	{
		// dispatch the event
		if (frameOutputEventDispatch)
		{
			frameOutputEventDispatch->tick();
			frameCount += 1;
		}
		maxClockWaitCount = 0;
		isExternalAlive = false;
	}

	
	// Any call happening here, is just the internal clock running along side the external clock, 
	// we ignore it.
}
