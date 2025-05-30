#pragma once
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
#include "concurrent_queue.hpp"
#include "types.hpp"



namespace KEvents
{
	struct Lock 
	{
		std::mutex mtx;
		std::condition_variable cv;
		std::atomic<bool> condition;

		Lock() 
			:condition(false)
		{
		}

		void wait()
		{
			std::unique_lock<std::mutex> lck(mtx);
			cv.wait(lck,
				[&] {
					if (condition)
						return true;
					return false;
				});
			condition = false;
		}

		void wait_until(int time_ms)
		{
			std::unique_lock<std::mutex> lck(mtx);
			cv.wait_for(lck, std::chrono::milliseconds(time_ms), [&] {
				if (condition)
					return true;
				return false;
				});
			condition = false;
		}

		void notify()
		{
			condition = true;
			cv.notify_all();
		}
	};

	class RunnableThread
	{
	public:
		RunnableThread(int id=0);
		RunnableThread(RunnableThread&& other) noexcept;
		/**
		* @brief
		* Called when the runnable is first created
		*/
		virtual bool init();
		/**
		* @brief
		* Called to execute the runnable on a separate thread
		*/
		virtual void run();
		/**
		* @brief
		* Called to do cleanup
		*/
		virtual void exit();
		// Methods specific to KEvents
		
		/**
		* @brief 
		* This function appends a task to be executed on the threadpool.
		* Tasks can be queued if you are sure the execution time won't block too long,
		* otherwise, let the thread pool dish out tasks to other runnables while you are busy.s
		* @tparam F func - func must be a callable
		*/
		template<typename F>
		void appendTask(F func)
		{
			func();
			/*task_q.emplace(func);
			notify();*/
		}
		/**
		* @brief
		* Atomic check of whether the task is done executing.
		*/
		bool isTaskComplete();

		/**
		* @brief
		* Wait for the runnable to finish or give up after time out and check another runnable.
		*/
		void waitFor(uint time);
		void notifyAll();
		void notify();

	private:
		std::queue<std::function<void()>> task_q;
		std::atomic<bool> taskStatus, exitFlag;
		std::chrono::time_point<std::chrono::steady_clock> execTimeStart, execTimeEnd;
		int runnableId;
		short frameCount;
		Lock lck;
	};


	class ThreadPool
	{
	public:
		ThreadPool(ulong poolSize=6);
		~ThreadPool();
		void run();
		void start();
		void stop();
		template<typename F>
		void appendTask(F task) 
		{ 

			taskQ.push(task); 
			lck.notify(); 
		}
		
	private:
		concurrent::ConcurrentQueue<std::function<void()>> taskQ;
		ulong threadCount, poolSize;
		std::atomic<bool>exitFlag;
		std::vector<RunnableThreadPtr> runnables;
		std::vector<std::thread*> workers;
		std::thread* poolWorker;
		long maxTries;
		Lock lck;
		std::chrono::time_point<std::chrono::steady_clock> execTimeStart, execTimeEnd;
	};
}