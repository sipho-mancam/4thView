#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include "concurrent_queue.hpp"
#include "types.hpp"



namespace KEvents
{
	class RunnableThread
	{

	};


	class ThreadPool
	{
	public:
		ThreadPool(ulong poolSize);
	};
}