#pragma once
#include "IScheduledWorker.hpp"
#include "Thread.hpp"

typedef uint32_t MilliSecond;
namespace Concurrency
{
    class IScheduler
    {
    public:
        virtual void Attach(IScheduledWorker& scheduleItem, MilliSecond interval, ThreadPriority threadPriority) = 0;
        virtual void Attach(IScheduledWorker& scheduleItem, ThreadPriority threadPriority, MilliSecond interval,
                            MilliSecond duration) = 0;
        virtual void Attach(const char* name, Action action, MilliSecond interval,
                            const ThreadPriority threadPriority) = 0;
        virtual void Attach(const char* name, Action action, MilliSecond interval, const ThreadPriority threadPriority,
                            TimeoutCallback callback) = 0;
        virtual ~IScheduler() = default;
    };

    class IRoutine
    {
    public:
        virtual void PerformInboundRoutine() = 0;
        virtual void PerformOutboundRoutine() = 0;
        virtual ~IRoutine() = default;
    };
} // namespace Concurrency
