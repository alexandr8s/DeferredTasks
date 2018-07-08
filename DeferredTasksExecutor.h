#ifndef DEFERRED_TASKS_EXECUTOR_H
#define DEFERRED_TASKS_EXECUTOR_H

#include <functional>
#include <future>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>

template <class T>
class DeferredTasksExecutor
{
    bool cond_ready;
	bool destroy_threads;
    std::mutex m_qeue;
    std::mutex m_active;
    std::condition_variable active_cond;
    std::vector<std::thread> threads;
    std::queue<std::packaged_task<T>> tasks;
public:
    DeferredTasksExecutor(int thread_count = std::thread::hardware_concurrency());
    ~DeferredTasksExecutor();
    void submit(std::packaged_task<T> task);
private:
    void worker();
};

#include "DeferredTasksExecutor.cpp"

#endif