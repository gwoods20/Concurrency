#pragma once
#include <string>

namespace Concurrency
{
    enum class LogLevel : int
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error
    };
    class ILogSinker
    {
    public:
        virtual void Log(const LogLevel level, const std::string& message) = 0;
        virtual ~ILogSinker() = default;
    };

    class ConcurrencyLog 
    {
    public:
        void RegisterSinker(ILogSinker* sinker);
        void Log(const LogLevel level, const std::string& message);
        ~ConcurrencyLog();
        static ConcurrencyLog& GetInstance();
    private:
        ConcurrencyLog();
        ILogSinker* sinker;
        bool started;
    };
} // namespace Concurrency