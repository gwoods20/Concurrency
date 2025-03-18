
#pragma once
#include <Concurrency/Thread.hpp>
#include "IScheduledWorker.hpp"
#include "IScheduler.hpp"
#include "RoutineTimeMonitor.hpp"

#include <condition_variable>
namespace Concurrency
{
    class CyclicalWorker : public IRunnable
    {
    public:
        CyclicalWorker(IScheduledWorker& scheduleItem, MilliSecond interval, const ThreadPriority threadPriority,
                       MilliSecond duration = 0);
        ~CyclicalWorker();
        void ScheduleWork();

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
