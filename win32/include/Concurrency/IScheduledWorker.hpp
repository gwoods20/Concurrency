#pragma once
#include <functional>
#include <string>

namespace Concurrency
{
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
     * @brief A final class representing a scheduled worker agent that inherits from IScheduledWorker.
     * 
     * This class is designed to manage a scheduled worker with a specific name, action, and optional timeout callback.
     * It provides methods to run the worker once, get the worker's name, and notify about duration timeouts.
     */
    class ScheduledWorkerAgent final : public IScheduledWorker
    {
    public:
        /**
         * @brief Construct a new Scheduled Worker Agent object with a name, action, and timeout callback.
         * 
         * @param name The name of the scheduled worker.
         * @param action The action to be executed by the worker.
         * @param callback The callback function to be called when a duration timeout occurs.
         */
        ScheduledWorkerAgent(const char* name, Action action, TimeoutCallback callback);

        /**
         * @brief Construct a new Scheduled Worker Agent object with a name and action.
         * 
         * @param name The name of the scheduled worker.
         * @param action The action to be executed by the worker.
         */
        ScheduledWorkerAgent(const char* name, Action action);

        /**
         * @brief Destroy the Scheduled Worker Agent object.
         * 
         * This is the overridden destructor from the base class IScheduledWorker.
         */
        ~ScheduledWorkerAgent() override = default;

        /**
         * @brief Runs the worker's action once.
         * 
         * This method overrides the pure virtual method from the base class IScheduledWorker.
         */
        void RunOnce() override;

        /**
         * @brief Gets the name of the scheduled worker.
         * 
         * @return const char* The name of the scheduled worker.
         * 
         * This method overrides the pure virtual method from the base class IScheduledWorker.
         */
        const char* GetWorkerName() const override;

        /**
         * @brief Notifies the worker about a duration timeout.
         * 
         * @param isTimeout A boolean indicating whether a duration timeout has occurred.
         * 
         * This method overrides the pure virtual method from the base class IScheduledWorker.
         */
        void NotifyDurationTimeout(const bool& isTimeout) const override;

    private:
        std::string name;
        Action action;
        TimeoutCallback callback;
        bool callReady;
    };

} // namespace Concurrency
