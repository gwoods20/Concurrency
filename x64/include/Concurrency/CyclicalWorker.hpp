
#pragma once
#include <Concurrency/Thread.hpp>
#include "IScheduledWorker.hpp"
#include "IScheduler.hpp"
#include "RoutineTimeMonitor.hpp"

#include <condition_variable>
namespace Concurrency
{
    /**
     * @brief A class representing a cyclical worker that inherits from IRunnable.
     * 
     * This class is designed to manage a scheduled worker that runs cyclically.
     * It provides methods to start the work and control its execution.
     */
    class CyclicalWorker : public IRunnable
    {
    public:
        /**
         * @brief Construct a new Cyclical Worker object.
         * 
         * @param scheduleItem The scheduled worker to be executed cyclically.
         * @param interval The interval between each execution in milliseconds.
         * @param threadPriority The priority of the thread on which the worker will run.
         * @param duration The total duration for which the worker should run, default is 0.
         */
        CyclicalWorker(IScheduledWorker& scheduleItem, MilliSecond interval, const ThreadPriority threadPriority,
                       MilliSecond duration = 0);

        /**
         * @brief Destroy the Cyclical Worker object.
         */
        ~CyclicalWorker();

        /**
         * @brief Schedule the work to be executed cyclically.
         */
        void ScheduleWork();

        /**
         * @brief Override the Run method from IRunnable.
         * 
         * This method contains the main logic for running the worker cyclically.
         * 
         * @return true if the worker should continue running, false otherwise.
         */
        virtual bool Run() override;

    private:
        static constexpr uint32_t DURATION_MSG_INTERVAL = 60 * 1;
        std::chrono::milliseconds GetRequiredWaitTime() const;
        IScheduledWorker* hostWorker;
        std::chrono::milliseconds interval;
        uint32_t executionErrorsCnt;

        std::mutex mutex;
        std::condition_variable cond;
        bool terminated;
        RoutineTimeMonitor timeMonitor;

        Thread workThread;
        uint32_t scheduledCount;
        std::chrono::high_resolution_clock::time_point last;
        bool started;
        uint32_t msgCnt;
    };
} // namespace Concurrency
