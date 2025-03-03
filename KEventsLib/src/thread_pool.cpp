#include "thread_pool.hpp"


namespace KEvents
{
	ThreadPool::ThreadPool(ulong poolSz)
		:threadCount(0),
		poolSize(poolSz),
		maxTries(3),
		poolWorker(nullptr)
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

		start();
	}

	ThreadPool::~ThreadPool()
	{
		std::cout << "Shutting down thread pool ..." << std::endl;
		stop();
		

		for (auto& worker : workers)
		{
			if(worker->joinable())
				worker->join();

			delete worker;
		}
		std::cout << "Thread Pool shutdown complete..." << std::endl;
	}

	void ThreadPool::run()
	{
		while (!exitFlag)
		{
			lck.wait();
			if (!taskQ.empty())
			{
				try
				{
					std::function<void()> task;
					task = taskQ.pull();

					// find a runnable that's currently done with work
					bool escape = false;

					// strategy 1: Look for a Runnable that's finished or wait for one to finish up.
					for (int tries = 0; tries < maxTries; tries++)
					{
						for (RunnableThreadPtr runnable : runnables)
						{
							if (runnable->isTaskComplete())
							{
								runnable->appendTask(std::move(task));
								escape = true;
								break;
							}
						}

						if (escape)
						{
							// break out of the second loop
							break;
						}
						else
						{
							// retry durations ... 50ms intervals
							std::this_thread::sleep_for(std::chrono::milliseconds(10));
						}
					}
					// This means that strategy 1 failed, we couldn't find any free thread, now we
					// attempt to wait for 100ms for each runnable and then queue the task somewhere and leave.
					if (!escape)
					{
						// strategy 2: Wait and avg of 100ms
						int avgWaitTime = 100 / (int)runnables.size();
						for (RunnableThreadPtr runnable : runnables)
						{
							runnable->waitFor(avgWaitTime);
							if (runnable->isTaskComplete())
							{
								runnable->appendTask(std::move(task));
								escape = true;
								break;
							}
						}

						if (!escape)
						{
							for (RunnableThreadPtr runnable : runnables)
							{
								runnable->appendTask(std::move(task));
								escape = true;
								break;
							}
						}
					}
				}
				catch (std::runtime_error& re)
				{
					std::cerr << re.what() << std::endl;
					continue;
				}
			}
		}
	}

	void ThreadPool::start()
	{
		if (!poolWorker)
		{
			poolWorker = new std::thread(&ThreadPool::run, this);
		}
	}

	void ThreadPool::stop()
	{
		exitFlag = true;
		lck.notify();

		for (auto& runnable : runnables)
		{
			runnable->exit();
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
			// wait to be notified that you have a task to execute .. 
			// this is meant to reduce the processor time used for polling an empty queue.
			lck.wait();
			if (task_q.empty())
			{
				taskStatus = true;
				lck.wait_until(20);
				continue;
			}

			auto currentTask = task_q.front();
			taskStatus = false;
			currentTask();
			task_q.pop();
			//notifyAll();
			taskStatus = true;
		}
	}

	void RunnableThread::exit()
	{
		exitFlag = true;
		lck.notify();
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

	void RunnableThread::notify()
	{
		lck.notify();
	}
}

