#pragma once
#include "ITask.hpp"
#include <chrono>
#include <functional>

namespace Concurrency
{
    using Millisecond = uint64_t;
    using Microsecond = uint64_t;
    /**
     * @brief Interface for scheduled workers.
     * 
     * This interface defines the contract for classes that represent scheduled workers.
     * Scheduled workers are designed to perform a specific action at a scheduled time.
     */
    class IScheduledWorker
    {
    public:
        /**
         * @brief Runs the worker's action once.
         * 
         * This pure virtual method must be implemented by derived classes.
         * It is responsible for executing the main task of the scheduled worker.
         */
        virtual void RunOnce() = 0;

        /**
         * @brief Gets the name of the scheduled worker.
         * 
         * This pure virtual method must be implemented by derived classes.
         * It returns a const pointer to a null-terminated string representing the name of the worker.
         * 
         * @return const char* The name of the scheduled worker.
         */
        virtual const char* GetWorkerName() const = 0;

        /**
         * @brief Notifies the worker about a duration timeout.
         * 
         * This pure virtual method must be implemented by derived classes.
         * It is called when a duration timeout occurs during the execution of the worker.
         * 
         * @param isTimeout A boolean indicating whether a duration timeout has occurred.
         */
        virtual void NotifyDurationTimeout(const bool& isTimeout) const = 0;
        virtual ~IScheduledWorker() = default;

    protected:
        IScheduledWorker() {}
    };
    using Action = std::function<void()>;
    // TimeoutCallback
    using TimeoutCallback = std::function<void(const bool&)>;
    /**
     * @brief Interface for a scheduler.
     * 
     * This interface defines the contract for classes that implement a scheduler.
     * A scheduler is responsible for attaching scheduled workers and managing their execution.
     */
    class IScheduler
    {
    public:
        /**
         * @brief Attaches a scheduled worker to the scheduler with a specified interval and thread priority.
         * 
         * @param scheduleItem The scheduled worker to be attached.
         * @param interval The interval in milliseconds between each execution of the worker.
         * @param threadPriority The priority of the thread on which the worker will run.
         */
        virtual void Attach(IScheduledWorker& scheduleItem, Millisecond interval, TaskPriority threadPriority) = 0;

        /**
         * @brief Attaches a scheduled worker to the scheduler with a specified thread priority, interval, and duration.
         * 
         * @param scheduleItem The scheduled worker to be attached.
         * @param threadPriority The priority of the thread on which the worker will run.
         * @param interval The interval in milliseconds between each execution of the worker.
         * @param duration The total duration in milliseconds for which the worker will run.
         */
        virtual void Attach(IScheduledWorker& scheduleItem, TaskPriority threadPriority, Millisecond interval,
                            Millisecond duration) = 0;

        /**
         * @brief Attaches a task with a specified name, action, interval, and thread priority to the scheduler.
         * 
         * @param name The name of the task.
         * @param action The action to be executed by the task.
         * @param interval The interval in milliseconds between each execution of the task.
         * @param threadPriority The priority of the thread on which the task will run.
         */
        virtual void Attach(const char* name, Action action, Millisecond interval,
                            const TaskPriority threadPriority) = 0;

        /**
         * @brief Attaches a task with a specified name, action, interval, thread priority, and timeout callback to the scheduler.
         * 
         * @param name The name of the task.
         * @param action The action to be executed by the task.
         * @param interval The interval in milliseconds between each execution of the task.
         * @param threadPriority The priority of the thread on which the task will run.
         * @param callback The callback function to be called if the task execution times out.
         */
        virtual void Attach(const char* name, Action action, Millisecond interval, const TaskPriority threadPriority,
                            TimeoutCallback callback) = 0;

        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~IScheduler() = default;
    };
} // namespace Concurrency
