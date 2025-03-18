
#pragma once
#include "IScheduledWorker.hpp"
#include "IScheduler.hpp"
#include "RoutineTimeMonitor.hpp"
#include "Thread.hpp"

namespace Concurrency
{
    /**
     * @brief A critical worker class that inherits from IRunnable.
     * 
     * This class is responsible for managing a scheduled worker with a specific interval
     * and thread priority. It ensures the worker runs in a separate thread and monitors
     * its execution time and errors.
     */
    class CriticalWorker : public IRunnable
    {
    public:
        /**
         * @brief Construct a new Critical Worker object.
         * 
         * @param scheduleItem Reference to the scheduled worker to be managed.
         * @param interval The interval at which the worker should be executed.
         * @param threadPriority The priority of the thread on which the worker will run.
         */
        CriticalWorker(IScheduledWorker& scheduleItem, MilliSecond interval, const ThreadPriority threadPriority);

        /**
         * @brief Destroy the Critical Worker object.
         */
        ~CriticalWorker();

        /**
         * @brief Start the worker thread.
         * 
         * @return true if the thread started successfully, false otherwise.
         */
        virtual bool Start();

        /**
         * @brief Override the Run method from IRunnable.
         * 
         * This method contains the main logic for running the scheduled worker.
         * 
         * @return true if the worker should continue running, false otherwise.
         */
        virtual bool Run() override;

    private:
        IScheduledWorker* hostWorker;
        std::chrono::milliseconds interval;
        uint32_t executionErrorsCnt;

        bool terminated;
        RoutineTimeMonitor timeMonitor;

        Thread workThread;
        uint32_t scheduledCount;
        std::chrono::high_resolution_clock::time_point last;
        bool started;
    };
} // namespace Concurrency
