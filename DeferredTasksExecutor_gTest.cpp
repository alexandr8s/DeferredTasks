#include <ctime>
#include <unistd.h> 
#include <stdlib.h>
#include <vector>
#include <future>

#include "DeferredTasksExecutor.h"
#include "gtest/gtest.h"

#define random(r) (rand() % r)

TEST(DeferredTasksExecutor, createInstance)
{
	DeferredTasksExecutor<void()> * executor1 = new DeferredTasksExecutor<void()>(1);
	DeferredTasksExecutor<void()> * executor2 = new DeferredTasksExecutor<void()>(2);

	EXPECT_NE(executor1, executor2);

	delete executor2;
	delete executor1;
}

TEST(DeferredTasksExecutor, submitTask)
{
	DeferredTasksExecutor<int()> executor;
	std::packaged_task<int()> task([]{ return 42; }) ;
    std::future<int> result = task.get_future();
    executor.submit(std::move(task));

	EXPECT_EQ(42, result.get());
}

TEST(DeferredTasksExecutor, submitTasks)
{
	DeferredTasksExecutor<int()> executor;
	auto f = [](int i){ return i; };

	std::packaged_task<int()> task1(std::bind(f, 1));
	std::packaged_task<int()> task2(std::bind(f, 2));
	std::packaged_task<int()> task3(std::bind(f, 3));

    std::future<int> result1 = task1.get_future();
	std::future<int> result2 = task2.get_future();
	std::future<int> result3 = task3.get_future();

    executor.submit(std::move(task1));
	executor.submit(std::move(task2));
	executor.submit(std::move(task3));

	EXPECT_EQ(1, result1.get());
	EXPECT_EQ(2, result2.get());
	EXPECT_EQ(3, result3.get());
}

TEST(DeferredTasksExecutor, FIFO)
{
	DeferredTasksExecutor<std::time_t()> executor(1);

	std::packaged_task<std::time_t()> task1([]{return std::time(nullptr);});
	std::packaged_task<std::time_t()> task2([]{return std::time(nullptr);});

    std::future<std::time_t> result1 = task1.get_future();
	std::future<std::time_t> result2 = task2.get_future();

    executor.submit(std::move(task1));
	executor.submit(std::move(task2));

	ASSERT_FALSE(result1.get() < result2.get());
}

TEST(DeferredTasksExecutor, randTasks)
{
	DeferredTasksExecutor<void()> executor;
    std::vector<std::future<void>> tasks;

	for (int task_count = 0; task_count < 1000; task_count++)
    {
        std::packaged_task<void()> task(std::bind(usleep, random(1000)));
        std::future<void> result = task.get_future();
        executor.submit(std::move(task));

        tasks.push_back(std::move(result));
    }

	for (auto & task : tasks)
	{
        task.wait();
        ASSERT_TRUE(task.valid());
	}
}

int main(int argc, char **argv)
{
  printf("Running main() from DeferredTasksExecutor_gTest.cpp\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
