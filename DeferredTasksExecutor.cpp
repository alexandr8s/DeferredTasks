#include "DeferredTasksExecutor.h"

template <class T>
DeferredTasksExecutor<T>::DeferredTasksExecutor(int thread_count)
{
    cond_ready = false;
    destroy_threads = false;
    for (int i = 0; i < thread_count; i++)
    {
        threads.emplace_back(std::thread(&DeferredTasksExecutor::worker, this));
    }
}

template <class T>
DeferredTasksExecutor<T>::~DeferredTasksExecutor()
{
    destroy_threads = true;
    active_cond.notify_all();
    for (auto & t : threads)
        t.join();
}

template <class T>
void DeferredTasksExecutor<T>::submit(std::packaged_task<T> task)
{
    std::lock_guard<std::mutex> lock(m_qeue);
    tasks.emplace(std::move(task));
    
    cond_ready = true;
    active_cond.notify_all();
}

template <class T>
void DeferredTasksExecutor<T>::worker()
{
    while (!destroy_threads)
    {
        std::unique_lock<std::mutex> lock(m_active);
        active_cond.wait(lock, [this]() {return cond_ready || destroy_threads; });
        if (destroy_threads) break;

        std::packaged_task<T> task;
        m_qeue.lock();
        if(!tasks.empty())
        {
            task = std::move(tasks.front());
            tasks.pop();
        }
        else
        {
            cond_ready = false;
            continue;
        }
        m_qeue.unlock();
        task();
    }
}