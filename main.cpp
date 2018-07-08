#include <unistd.h> 
#include <stdlib.h>
#include <vector>
#include <future>
#include "DeferredTasksExecutor.h"

#define random(r) (rand() % r)

int main()
{
    DeferredTasksExecutor<void()> executor;
    std::vector<std::future<void>> tasks;

    for (int task_count = 0; task_count < 10; task_count++)
    {
        std::packaged_task<void()> task(std::bind(usleep, random(1000000)));
        std::future<void> result = task.get_future();
        executor.submit(std::move(task));

        tasks.push_back(std::move(result));
    }

    for (auto & task : tasks)
        task.wait();
    
    return 0;
}