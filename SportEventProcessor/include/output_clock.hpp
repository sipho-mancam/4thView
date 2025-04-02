#pragma once
#include "kevents.hpp"
#include "frame_output_event_dispatch.hpp"

#define INTERMEDIATE_WAKES 10
#define MAX_EXTERNAL_TICK_WAIT_COUNT 5
/**
* @brief
 *This class is responsible for managing the output clock. 
 * There are primarily 2 sources of timing in the system,
 * 1. the input clock from the video source
 * 2. internal clock that's configured from set fps in configuration.
*/
class OutputClock
{
public:
	OutputClock(std::shared_ptr<FrameOutputEventDispatch> fo);
	~OutputClock();

	/**
	* @brief
	 * This function is called when the external clock is updated.
	 * This function is responsible for keeping the system using the external clock
	*/
	void externalFrameUpdate();
	void __run_internalClock();
private:
	json config;
	bool isInternalClockSource, isExternalAlive, exit;
	long internalClockFrequency;
	// the number of times the internal clock needs to fire before the external clock fires
	// For the clock to switch to internal clocking system.
	std::atomic<int> maxClockWaitCount; 
	std::mutex clockMutex;
	int tickInterval;
	std::string moduleName;
	std::chrono::time_point<std::chrono::high_resolution_clock> startPoint, currentPoint;
	std::thread* clockWorker;
	std::shared_ptr<FrameOutputEventDispatch> frameOutputEventDispatch;

	void __dispatchOutputEvent();

};