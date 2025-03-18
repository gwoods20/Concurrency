#pragma once
#include <functional>
#include <string>

namespace Concurrency
{
    class IScheduledWorker
    {
    public:
        virtual void RunOnce() = 0;
        virtual const char* GetWorkerName() const = 0;
        virtual void NotifyDurationTimeout(const bool& isTimeout) const = 0;
        virtual ~IScheduledWorker() = default;

    protected:
        IScheduledWorker() {}
    };

    using Action = std::function<void()>;
    // TimeoutCallback
    using TimeoutCallback = std::function<void(const bool&)>;

    class ScheduledWorkerAgent final : public IScheduledWorker
    {
    public:
        ScheduledWorkerAgent(const char* name, Action action, TimeoutCallback callback);
        ScheduledWorkerAgent(const char* name, Action action);
        ~ScheduledWorkerAgent() override = default;

        void RunOnce() override;

        const char* GetWorkerName() const override;
        void NotifyDurationTimeout(const bool& isTimeout) const override;

    private:
        std::string name;
        Action action;
        TimeoutCallback callback;
        bool callReady;
    };

} // namespace Concurrency
