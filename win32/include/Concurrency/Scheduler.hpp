#pragma once
#include "CyclicalWorker.hpp"
#include "IScheduler.hpp"

#include <memory>
#include <mutex>
#include <vector>
namespace Concurrency
{
    class Scheduler : public IScheduler, public IRunnable
    {
    public:
        explicit Scheduler(ThreadPriority workerThreadPriority);
        ~Scheduler(void) override;

        void Attach(IScheduledWorker& scheduleItem, MilliSecond interval, const ThreadPriority threadPriority) override;
        void Attach(IScheduledWorker& scheduleItem, const ThreadPriority threadPriority, MilliSecond interval,
                    MilliSecond duration) override;

        virtual void Attach(const char* name, Action action, MilliSecond interval,
                            const ThreadPriority threadPriority) override;

        virtual void Attach(const char* name, Action action, MilliSecond interval, const ThreadPriority threadPriority,
                            TimeoutCallback callback) override;

        void Activate();
        void Deactivate();
        bool Run() override;

    private:
        typedef std::shared_ptr<CyclicalWorker> Item;
        typedef std::vector<Item> ScheduleContainer;
        typedef std::shared_ptr<IScheduledWorker> WorkerAgentItem;
        typedef std::vector<WorkerAgentItem> WorkerAgentContainer;

        void DeleteThread(Item workerThread) const;

        Thread thread;
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
