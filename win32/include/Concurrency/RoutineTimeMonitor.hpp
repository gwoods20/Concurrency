#pragma once

#include <chrono>

typedef uint64_t Microsecond;
#define MicrosecondInMillisecond 1000
#define MillisecondsInSecond 1000
namespace Concurrency
{
    class RoutineTimeMonitor
    {
    public:
        RoutineTimeMonitor(const Microsecond& expectedDuration, const Microsecond& expectedInterval);
        void Start();
        void Stop();

        Microsecond GetMaxDuration() const;
        Microsecond GetMinDuration() const;
        Microsecond GetCurrentDuration() const;
        Microsecond GetMaxInterval() const;
        Microsecond GetMinInterval() const;
        Microsecond GetCurrentInterval() const;

        uint64_t GetElapsedFaultCount() const;
        uint64_t GetIntervalFaultCount() const;
        void IncrementIntervalFaultCount();

        bool Is1StLoopDone() const;

        virtual void ResetElapsedTiming(const bool& reset);
        virtual void ResetIntervalTiming(const bool& reset);

    private:
        bool IsIntervalTimeout() const;
        bool IsDurationTimeout() const;

        Microsecond durationMax;
        Microsecond durationMin;
        Microsecond durationCur;
        Microsecond intervalMax;
        Microsecond intervalMin;
        Microsecond intervalCur;

        const Microsecond durationExpt;
        const Microsecond intervalExpt;

        uint64_t durationFaultCount;
        uint64_t intervalFaultCount;

        const Microsecond durationDeviation;
        const Microsecond intervalDeviation;

        std::chrono::high_resolution_clock::time_point startTime;
        bool firstLoopDone;
    };
} // namespace Concurrency
