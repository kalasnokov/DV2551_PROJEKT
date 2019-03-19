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
	std::mutex queue_mutex;
	void infiniteLoop()
	{
		while (true)
		{
			std::function<void()> job = nullptr;
			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				if (!work.empty())
				{
					job = work.front();
				}
			}
			if (!job)
			{
				job();
			}
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
			std::unique_lock<std::mutex> lock(queue_mutex);
			work.push(job);
		}
	}
};