
#pragma once
#include "IScheduledWorker.hpp"
#include "IScheduler.hpp"
#include "RoutineTimeMonitor.hpp"
#include "Thread.hpp"

namespace Concurrency
{
    class CriticalWorker : public IRunnable
    {
    public:
        CriticalWorker(IScheduledWorker& scheduleItem, MilliSecond interval, const ThreadPriority threadPriority);
        ~CriticalWorker();

        virtual bool Start();
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
