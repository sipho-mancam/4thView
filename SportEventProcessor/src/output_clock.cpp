#include "output_clock.hpp"

OutputClock::OutputClock()
	: isInternalClockSource(false), 
	isExternalAlive(true), 
	exit(false),
	internalClockFrequency(0), 
	maxClockWaitCount(0)
{
	config = KEvents::__load_config__();
	moduleName = "SportEventProcessor";
	tickInterval = config[moduleName]["clock_tick_interval_ms"];

}

OutputClock::~OutputClock()
{

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
	if (maxClockWaitCount == 0 || maxClockWaitCount >= MAX_EXTERNAL_TICK_WAIT_COUNT)
	{
		// dispatch the event
		std::cout << "Output Event Dispatched" << std::endl;
	}
	// Any call happening here, is just the internal clock running along side the external clock, 
	// we ignore it.
	maxClockWaitCount = 0;
}
