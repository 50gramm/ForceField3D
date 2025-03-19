#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>


class ThreadPool
{
public:
	using Task = std::function<void()>;

	ThreadPool(size_t numThreads = std::max(1U, std::thread::hardware_concurrency()-1))
	{
		for(size_t i = 0; i < numThreads; ++i)
		{
			threads.emplace_back([this]
			{
				while (true)
				{
					Task task;

					{
						std::unique_lock<std::mutex> lock(queuMutex);

						// Waiting until there is a task to
						// execute or the pool is stopped
						cv.wait(lock, [this] {
							return !tasks.empty() || stop;
						});

						// Exit the thread in case the pool
						// is stopped and there are no tasks
						if (stop && tasks.empty()) {
							return;
						}

						// Get the next task from the queue
						task = std::move(tasks.front());
						tasks.pop();
					}

					cv.notify_one();

					task();
				}
			});
		}
	}


	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(queuMutex);
			stop = true;
		}

		cv.notify_all();

		for (auto& thread : threads) {
			thread.join();
		}
	}


	void addTask(Task task)
	{
		{
			std::unique_lock<std::mutex> lock(queuMutex);
			// cv.wait(lock, [this] {
			// 	return tasks.size() < threads.size();
			// });
			tasks.emplace(std::move(task));
		}

		cv.notify_one();
	}


private:
	mutable std::mutex queuMutex;
	std::condition_variable cv;
	std::vector<std::thread> threads;
	std::queue<Task> tasks;
	bool stop = false;
};
