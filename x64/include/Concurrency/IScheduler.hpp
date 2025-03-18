#pragma once
#include "IScheduledWorker.hpp"
#include "Thread.hpp"

typedef uint32_t MilliSecond;
namespace Concurrency
{
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
        virtual void Attach(IScheduledWorker& scheduleItem, MilliSecond interval, ThreadPriority threadPriority) = 0;

        /**
         * @brief Attaches a scheduled worker to the scheduler with a specified thread priority, interval, and duration.
         * 
         * @param scheduleItem The scheduled worker to be attached.
         * @param threadPriority The priority of the thread on which the worker will run.
         * @param interval The interval in milliseconds between each execution of the worker.
         * @param duration The total duration in milliseconds for which the worker will run.
         */
        virtual void Attach(IScheduledWorker& scheduleItem, ThreadPriority threadPriority, MilliSecond interval,
                            MilliSecond duration) = 0;

        /**
         * @brief Attaches a task with a specified name, action, interval, and thread priority to the scheduler.
         * 
         * @param name The name of the task.
         * @param action The action to be executed by the task.
         * @param interval The interval in milliseconds between each execution of the task.
         * @param threadPriority The priority of the thread on which the task will run.
         */
        virtual void Attach(const char* name, Action action, MilliSecond interval,
                            const ThreadPriority threadPriority) = 0;

        /**
         * @brief Attaches a task with a specified name, action, interval, thread priority, and timeout callback to the scheduler.
         * 
         * @param name The name of the task.
         * @param action The action to be executed by the task.
         * @param interval The interval in milliseconds between each execution of the task.
         * @param threadPriority The priority of the thread on which the task will run.
         * @param callback The callback function to be called if the task execution times out.
         */
        virtual void Attach(const char* name, Action action, MilliSecond interval, const ThreadPriority threadPriority,
                            TimeoutCallback callback) = 0;

        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~IScheduler() = default;
    };

    /**
     * @brief Interface for a routine.
     * 
     * This interface defines the contract for classes that implement a routine.
     * A routine consists of an inbound and an outbound operation.
     */
    class IRoutine
    {
    public:
        /**
         * @brief Performs the inbound routine.
         * 
         * This method must be implemented by derived classes to define the inbound operation.
         */
        virtual void PerformInboundRoutine() = 0;

        /**
         * @brief Performs the outbound routine.
         * 
         * This method must be implemented by derived classes to define the outbound operation.
         */
        virtual void PerformOutboundRoutine() = 0;

        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~IRoutine() = default;
    };
} // namespace Concurrency
