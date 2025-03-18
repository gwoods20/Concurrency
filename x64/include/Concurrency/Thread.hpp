#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <thread>
namespace Concurrency
{
    class IRunnable
    {
    public:
        virtual ~IRunnable() = default;
        virtual bool Run() = 0;
    };

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define OSI_PLATFORM Win32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#include <windows.h>

    using pid_t = HANDLE;
    using thread_t = HANDLE;
    using thread_id_t = DWORD;
    using ThreadPriority = int32_t;
    using ThreadSchedPolicy = uint32_t;
#define THREAD_PRIO_ABOVE_NORMAL THREAD_PRIORITY_ABOVE_NORMAL
#define THREAD_PRIO_BELOW_NORMAL THREAD_PRIORITY_BELOW_NORMAL
#define THREAD_PRIO_HIGHEST THREAD_PRIORITY_HIGHEST
#define THREAD_PRIO_IDLE THREAD_PRIORITY_IDLE
#define THREAD_PRIO_LOWEST THREAD_PRIORITY_LOWEST
#define THREAD_PRIO_NORMAL THREAD_PRIORITY_NORMAL
#define THREAD_PRIO_TIME_CRITICAL THREAD_PRIORITY_TIME_CRITICAL
#define THREAD_SCHED_NRT_TS 0
#define THREAD_SCHED_RT_FIFO 1
#define THREAD_SCHED_RT_RR 2
#define THREAD_SCHED_DEFAULT THREAD_SCHED_NRT_TS
#else
#define OSI_PLATFORM Posix
#include <pthread.h>
    typedef ::pid_t pid_t;
    typedef uint32_t ThreadPriority;
    typedef pthread_t thread_t;
    typedef pthread_t thread_id_t;
    typedef uint32_t ThreadSchedPolicy;
#define THREAD_PRIO_ABOVE_NORMAL 20
#define THREAD_PRIO_BELOW_NORMAL 8
#define THREAD_PRIO_HIGHEST 30
#define THREAD_PRIO_IDLE 0
#define THREAD_PRIO_LOWEST 5
#define THREAD_PRIO_NORMAL 10
#define THREAD_PRIO_TIME_CRITICAL 40
#define THREAD_SCHED_RT_RR SCHED_RR
#define THREAD_SCHED_RT_FIFO SCHED_FIFO
#define THREAD_SCHED_NRT_TS SCHED_OTHER
#define THREAD_SCHED_DEFAULT SCHED_RR

    typedef pthread_t thread_t;
#endif

    thread_t GetThreadId();
    void SetPriority(thread_t thread, ThreadPriority priority, ThreadSchedPolicy policy = THREAD_SCHED_DEFAULT);
    void SetThisThreadPriority(ThreadPriority priority);
    void SetStackSize(uint32_t stackSize);
    ThreadPriority GetPriority(thread_t thread);
    void SetThisThreadName(const char* name);
    std::string GetThisThreadName();

    using ThreadEntry = std::function<void()>;

    /**
     * @brief Interface for a thread abstraction.
     * 
     * This interface defines the common behavior for all thread implementations.
     * It provides methods to start a thread, check if it is running, and wait for it to complete.
     */
    class IThread
    {
    public:
        enum PRIO
        {
            TimeCriticalPrio = 70,
            HighestPrio = 60,
            AboveNormalPrio = 50,
            NormalPrio = 30,
            BelowNormalPrio = 20,
            LowestPrio = 10,
        };

        virtual ~IThread() = default;
        /**
         * @brief Starts the thread.
         * 
         * This method initializes and starts the execution of the thread.
         * It must be implemented by derived classes.
         */
        virtual void Start() = 0;
        /**
         * @brief Checks if the thread is currently running.
         * 
         * This method returns a boolean value indicating whether the thread is actively executing.
         * It must be implemented by derived classes.
         * 
         * @return true if the thread is running, false otherwise.
         */
        virtual bool IsRunning() = 0;
        /**
         * @brief Waits for the thread to complete its execution.
         * 
         * This method blocks the calling thread until the thread has finished executing.
         * It must be implemented by derived classes.
         */
        virtual void Join() = 0;
    };

    /**
     * @brief A concrete implementation of the IThread interface.
     * 
     * This class provides a platform-independent way to manage threads.
     * It inherits from the IThread interface and implements its methods.
     */
    class Thread : public IThread
    {
    public:
        /**
         * @brief Construct a new Thread object.
         * 
         * @param run A reference to an object implementing the IRunnable interface.
         * @param name The name of the thread.
         * @param stackSize The stack size of the thread.
         * @param prio The priority of the thread.
         * @param policy The scheduling policy of the thread, defaults to THREAD_SCHED_DEFAULT.
         */
        Thread(IRunnable& run, const std::string& name, uint32_t stackSize, ThreadPriority prio,
               ThreadSchedPolicy policy = THREAD_SCHED_DEFAULT);
        /**
         * @brief Copy constructor for the Thread object.
         * 
         * @param other The other Thread object to copy from.
         */
        Thread(const Thread&) = default;
        /**
         * @brief Destroy the Thread object.
         * 
         * This is the overridden destructor from the base class IThread.
         */
        ~Thread() override;
        /**
         * @brief Starts the thread.
         * 
         * This method initializes and starts the execution of the thread.
         * It overrides the pure virtual method from the base class IThread.
         */
        void Start() override;
        /**
         * @brief Checks if the thread is currently running.
         * 
         * This method returns a boolean value indicating whether the thread is actively executing.
         * It overrides the pure virtual method from the base class IThread.
         * 
         * @return true if the thread is running, false otherwise.
         */
        bool IsRunning() override;
        /**
         * @brief Waits for the thread to complete its execution.
         * 
         * This method blocks the calling thread until the thread has finished executing.
         * It overrides the pure virtual method from the base class IThread.
         */
        void Join() override;
        /**
         * @brief Checks if the thread is joinable.
         * 
         * @return true if the thread is joinable, false otherwise.
         */
        bool Joinable();

        /**
         * @brief Gets the operating system thread name.
         * 
         * @param threadHandle The handle of the thread.
         * @return const char* The name of the thread.
         */
        static const char* GetOsThreadName(thread_t threadHandle);

    private:
        void Run();
        Thread() = delete;

        Thread& operator=(const Thread&) = default;

        std::shared_ptr<std::thread> thread;
        IRunnable* instance;
        uint32_t stackSize;
        std::string name;
        ThreadPriority prio;
        ThreadSchedPolicy schedPolicy;
        thread_t threadId;
        bool isRunning;
    };

    /**
     * @brief A class representing a thread agent that inherits from IRunnable.
     * 
     * This class provides an interface to manage a thread with a specific name, priority, and scheduling policy.
     */
    class ThreadAgent : public IRunnable
    {
    public:
        /**
         * @brief Construct a new Thread Agent object.
         * 
         * @param name The name of the thread agent.
         * @param prio The priority of the thread.
         * @param policy The scheduling policy of the thread, default is THREAD_SCHED_NRT_TS.
         */
        explicit ThreadAgent(std::string name, ThreadPriority prio, ThreadSchedPolicy policy = THREAD_SCHED_NRT_TS);

        /**
         * @brief Start the thread with the given function.
         * 
         * @param f The function to be executed in the thread.
         * @return Thread The thread object.
         */
        Thread Start(std::function<void()> f);

        /**
         * @brief Check if the thread is currently running.
         * 
         * @return true if the thread is running, false otherwise.
         */
        bool IsRunning();

        /**
         * @brief Wait for the thread to complete its execution.
         */
        void Join();

        /**
         * @brief Check if the thread is joinable.
         * 
         * @return true if the thread is joinable, false otherwise.
         */
        bool Joinable();

        /**
         * @brief Override the Run method from IRunnable.
         * 
         * This method contains the main logic for running the thread.
         * 
         * @return true if the thread should continue running, false otherwise.
         */
        bool Run() override;

    private:
        Thread thread;
        std::function<void()> threadEntry;
    };
} // namespace Concurrency
