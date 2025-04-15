#pragma once

#include <chrono>
#include <cstdint>

#include "IScheduler.hpp"
#define MicrosecondInMillisecond 1000
#define MillisecondsInSecond 1000
namespace Concurrency
{
    /**
     * @brief A class used to monitor the time of a routine.
     * 
     * This class provides functionality to monitor the duration and interval of a routine.
     * It can record the maximum, minimum, and current duration and interval, as well as the number of faults.
     */
    class RoutineTimeMonitor
    {
    public:
        /**
         * @brief Construct a new Routine Time Monitor object.
         * 
         * @param expectedDuration The expected duration of the routine in microseconds.
         * @param expectedInterval The expected interval between routine executions in microseconds.
         */
        RoutineTimeMonitor(const Microsecond expectedDuration, const Microsecond expectedInterval);

        /**
         * @brief Start monitoring the routine time.
         * 
         * This method marks the start time of the routine.
         */
        void Start();

        /**
         * @brief Stop monitoring the routine time.
         * 
         * This method stops the monitoring and calculates the duration and interval.
         */
        void Stop();

        /**
         * @brief Get the maximum duration of the routine.
         * 
         * @return Microsecond The maximum duration in microseconds.
         */
        Microsecond GetMaxDuration() const;

        /**
         * @brief Get the minimum duration of the routine.
         * 
         * @return Microsecond The minimum duration in microseconds.
         */
        Microsecond GetMinDuration() const;

        /**
         * @brief Get the current duration of the routine.
         * 
         * @return Microsecond The current duration in microseconds.
         */
        Microsecond GetCurrentDuration() const;

        /**
         * @brief Get the maximum interval between routine executions.
         * 
         * @return Microsecond The maximum interval in microseconds.
         */
        Microsecond GetMaxInterval() const;

        /**
         * @brief Get the minimum interval between routine executions.
         * 
         * @return Microsecond The minimum interval in microseconds.
         */
        Microsecond GetMinInterval() const;

        /**
         * @brief Get the current interval between routine executions.
         * 
         * @return Microsecond The current interval in microseconds.
         */
        Microsecond GetCurrentInterval() const;

        /**
         * @brief Get the number of elapsed time faults.
         * 
         * @return uint64_t The number of elapsed time faults.
         */
        uint64_t GetElapsedFaultCount() const;

        /**
         * @brief Get the number of interval faults.
         * 
         * @return uint64_t The number of interval faults.
         */
        uint64_t GetIntervalFaultCount() const;

        /**
         * @brief Increment the number of interval faults.
         */
        void IncrementIntervalFaultCount();

        /**
         * @brief Check if the first loop of the routine is done.
         * 
         * @return true if the first loop is done, false otherwise.
         */
        bool Is1StLoopDone() const;

        /**
         * @brief Reset the elapsed timing information.
         * 
         * @param reset A boolean indicating whether to reset the elapsed timing.
         */
        virtual void ResetElapsedTiming(const bool& reset);

        /**
         * @brief Reset the interval timing information.
         * 
         * @param reset A boolean indicating whether to reset the interval timing.
         */
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
