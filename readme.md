Develop DeferredTasksExecutor class that
-	Owns a thread pool
-	Accepts DeferredTasks
-	Manages FIFO tasks queue
-	Processes task on first available idle thread
Client code should use a simple interface for submitting and waiting for deferred tasks.
Example tasks may be implemented as usleep(random(1000000)) simulating real heavy calculations

Advanced requirements:
-	Code is covered with unit tests (gtest, cppunit, qt test or similar)
-	Number of threads in the pool is reasonable for current machine
-	Clients have an interface to get the task progress (in queue, processing, done). 
-	Task has a start precondition - task does not start until precondition is met. 
        This allows sequential task processing - one task waits for other task completion
-	There is a way to cancel a task
-	Each task has execution priority