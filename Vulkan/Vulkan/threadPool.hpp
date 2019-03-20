#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <queue>
class threadPool
{
private:
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> work;
	std::mutex queueMutex;
	std::condition_variable condition;
	void infiniteLoop()
	{
		while (true)
		{
			std::function<void()> job;
			{
				std::unique_lock<std::mutex> lock(queueMutex);

				condition.wait(lock, [this] {return !work.empty(); });
				job = work.front();
				work.pop();
			}
			job();
		}
	}
public:
	threadPool()
	{
		const size_t num_processors = std::thread::hardware_concurrency();
		if (!num_processors)
		{
			throw std::exception("Could not find the amount of cores");
		}
		workers.resize(num_processors - 1);
		for (size_t i = 0; i < workers.size(); i++)
		{
			workers[i] = std::thread([this]{ this->infiniteLoop(); });
			workers[i].detach();
		}
	}
	~threadPool()
	{
		for (auto &worker : workers)
		{
			worker.join();
		}
	}
	void queueJob(std::function<void()> job)
	{
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			work.push(job);
			condition.notify_one();
		}
	}
	void wait()
	{
		while (!work.empty())
		{

		}
	}
};