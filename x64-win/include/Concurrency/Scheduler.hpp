#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>
#include "CyclicalWorker.hpp"
#include "IScheduler.hpp"
namespace Concurrency
{
    /**
     * @class Scheduler
     * @brief A class that manages the scheduling of tasks.
     *
     * The Scheduler class is responsible for attaching, activating, deactivating,
     * and running scheduled tasks. It inherits from IScheduler and ITask.
     */
    class Scheduler : public IScheduler, public ITask
    {
    public:
        /**
         * @brief Constructor for the Scheduler class.
         *
         * Initializes a new instance of the Scheduler class with the specified thread priority
         * for all worker threads managed by this scheduler.
         *
         * @param workerTaskPriority The priority of the worker threads to be managed by this scheduler.
         */
        explicit Scheduler(TaskPriority workerTaskPriority);
        ~Scheduler(void) override;

        /**
         * @brief Attaches a scheduled worker to the scheduler with a specified interval and thread priority.
         *
         * @param scheduleItem The scheduled worker to attach.
         * @param interval The interval at which the worker should be executed.
         * @param threadPriority The priority of the thread on which the worker will run.
         */
        void Attach(IScheduledWorker& scheduleItem, Millisecond interval, const TaskPriority threadPriority) override;

        /**
         * @brief Attaches a scheduled worker to the scheduler with a specified thread priority and interval.
         *
         * @param scheduleItem The scheduled worker to attach.
         * @param threadPriority The priority of the thread on which the worker will run.
         * @param interval The interval at which the worker should be executed.
         */
        void Attach(IScheduledWorker& scheduleItem, const TaskPriority threadPriority, Millisecond interval,
                    Millisecond duration) override;

        /**
         * @brief Attaches a task with a specified name, action, interval, and other parameters to the scheduler.
         *
         * This method is used to schedule a task with a given name and action to be executed at a specified interval.
         * Additional parameters can be provided to further configure the task's behavior.
         *
         * @param name The name of the task to be attached.
         * @param action The action to be executed at the specified interval.
         * @param interval The interval at which the action should be executed.
         */
        virtual void Attach(const char* name, Action action, Millisecond interval,
                            const TaskPriority threadPriority) override;

        /**
         * @brief Attaches a task with a specified name, action, interval, thread priority, and timeout callback to the
         * scheduler.
         *
         * This method is used to schedule a task with a given name and action to be executed at a specified interval.
         * The task will run on a thread with the specified priority. Additionally, a timeout callback can be provided
         * to handle cases where the task execution exceeds a certain time limit.
         *
         * @param name The name of the task to be attached.
         * @param action The action to be executed at the specified interval.
         * @param interval The interval at which the action should be executed.
         * @param threadPriority The priority of the thread on which the task will run.
         * @param callback The callback function to be called if the task execution times out.
         */
        virtual void Attach(const char* name, Action action, Millisecond interval, const TaskPriority threadPriority,
                            TimeoutCallback callback) override;

        /**
         * @brief Activates the scheduler and all attached workers.
         *
         * Starts processing scheduled tasks and enables execution
         * of worker threads according to their configured intervals.
         */
        void Activate();

        /**
         * @brief Deactivates the scheduler and all attached workers.
         *
         * Stops processing new tasks and suspends execution
         * of worker threads while maintaining their state.
         */
        void Deactivate();

        /**
         * @brief Executes the scheduler's main processing loop.
         * @return bool True if the scheduler continues running,
         *             false if termination was requested.
         *
         * Overrides ITask::Run() to implement the core scheduling
         * logic that manages worker thread execution and task timing.
         */
        bool Run() override;

    private:
        typedef std::shared_ptr<CyclicalWorker> Item;
        typedef std::vector<Item> ScheduleContainer;
        typedef std::shared_ptr<IScheduledWorker> WorkerAgentItem;
        typedef std::vector<WorkerAgentItem> WorkerAgentContainer;

        void DeleteThread(Item workerThread) const;

        std::shared_ptr<Thread> thread;
        bool terminated;
        bool active;
        ScheduleContainer workers;
        WorkerAgentContainer agents;
        std::mutex workersMutex;
        const uint32_t maxWorkers;
        const std::chrono::microseconds syncInterval;
        std::chrono::high_resolution_clock::time_point last;
        bool started;
    };
} // namespace Concurrency
