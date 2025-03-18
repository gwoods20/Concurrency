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
        virtual void Start() = 0;
        virtual bool IsRunning() = 0;
        virtual void Join() = 0;
    };

    class Thread : public IThread
    {
    public:
        Thread(IRunnable& run, const std::string& name, uint32_t stackSize, ThreadPriority prio,
               ThreadSchedPolicy policy = THREAD_SCHED_DEFAULT);
        Thread(const Thread&) = default;
        ~Thread() override;
        void Start() override;
        bool IsRunning() override;
        void Join() override;
        bool Joinable();

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

    class ThreadAgent : public IRunnable
    {
    public:
        explicit ThreadAgent(std::string name, ThreadPriority prio, ThreadSchedPolicy policy = THREAD_SCHED_NRT_TS);

        Thread Start(std::function<void()> f);
        bool IsRunning();
        void Join();
        bool Joinable();
        bool Run() override;

    private:
        Thread thread;
        std::function<void()> threadEntry;
    };
} // namespace Concurrency
