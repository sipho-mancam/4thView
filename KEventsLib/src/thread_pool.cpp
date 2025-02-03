#include "thread_pool.hpp"


namespace KEvents
{
	ThreadPool::ThreadPool(ulong poolSz)
		:threadCount(0),
		poolSize(poolSz)
	{
		for (int i = 0; i < poolSize; i++)
		{
			RunnableThreadPtr runnable = std::make_shared<RunnableThread>();
			if(runnable->init())
			{
				workers.push_back(new std::thread(&RunnableThread::run, runnable));
				runnables.push_back(runnable);
			}
		}
	}

	ThreadPool::~ThreadPool()
	{
		exitFlag = true;
		for (auto& runnable : runnables)
		{
			runnable->exit();
		}

		for (auto& worker : workers)
		{
			if(worker->joinable())
				worker->join();

			delete worker;
		}

	}

	void ThreadPool::run()
	{
		while (!exitFlag)
		{
			// Check if the queue is not empty and dish out tasks

		}
	}

	
	RunnableThread::RunnableThread()
		:taskStatus(false),
		exitFlag(false)
	{
	}

	RunnableThread::RunnableThread(RunnableThread&& other) noexcept
	{
		taskStatus = false;
		exitFlag = false;
	}

	bool RunnableThread::init()
	{
		return true;
	}

	void RunnableThread::run()
	{
		while (!exitFlag)
		{
			if (task_q.empty())
			{
				taskStatus = true;
				continue;
			}

			auto currentTask = task_q.front();
			taskStatus = false;
			currentTask();
			task_q.pop();
			notifyAll();
			taskStatus = true;
		}
	}

	void RunnableThread::exit()
	{
		exitFlag = true;
	}

	bool RunnableThread::isTaskComplete()
	{
		return taskStatus;
	}

	void RunnableThread::waitFor(uint time)
	{
		lck.wait_until(static_cast<int>(time));
	}

	void RunnableThread::notifyAll()
	{
		lck.notify();
	}


}

