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
			RunnableThreadPtr runnable = std::make_shared<RunnableThread>(i);
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
			execTimeStart = std::chrono::steady_clock::now();
			if (!taskQ.empty())
			{
				try
				{
					std::function<void()> task = taskQ.pull();
					if (!task)
						continue;

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
			else {
				lck.wait();
			}
			execTimeEnd = std::chrono::steady_clock::now();
			long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(execTimeEnd - execTimeStart).count();
			//printf("Elapsed time for a single runnable callback: %lld ms\n", elapsedTime);
		}
	}

	void ThreadPool::start()
	{
		KEvents::kEventsLogger->info("Starting thread pool ... with {} runnables.", runnables.size());
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

	
	RunnableThread::RunnableThread(int id)
		:taskStatus(false),
		exitFlag(false),
		runnableId(id),
		frameCount(0)
	{
	}

	RunnableThread::RunnableThread(RunnableThread&& other) noexcept
	{
		taskStatus = false;
		exitFlag = false;
		runnableId = other.runnableId;
		frameCount = 0;
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
			long long elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(execTimeEnd - execTimeStart).count();
			if (elapsedTime >= 1)
			{
				execTimeStart = std::chrono::steady_clock::now();
				double frameRate = (frameCount * 1.0) /(elapsedTime * 1.0) ;
				//KEvents::kEventsLogger->info("Runnable Id:{}, Frame rate:{} fps ... elapsed Time: {} seconds. Queue Length: {}", runnableId, frameRate, elapsedTime, task_q.size());
				frameCount = 0;
			}

			
			if (task_q.empty())
			{
				taskStatus = true;
				lck.wait();
			}

			// This means that notify was called from somewhere else that doesn't append tasks
			if (task_q.empty())
			{
				continue;
			}

			auto currentTask = task_q.front();
			taskStatus = false;
			task_q.pop();
			currentTask();
			taskStatus = true;
			execTimeEnd = std::chrono::steady_clock::now();
			frameCount++;
		}
	}

	void RunnableThread::exit()
	{
		exitFlag = true;
		lck.notify();
	}

	bool RunnableThread::isTaskComplete()
	{
		return task_q.empty();
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

