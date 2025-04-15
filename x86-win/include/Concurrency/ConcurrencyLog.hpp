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
    /**
     * @brief Interface for log sinkers.
     * 
     * This interface defines the contract for log sinkers, which are responsible for
     * handling and outputting log messages. Implementations of this interface can
     * write logs to various destinations such as files, consoles, or remote servers.
     */
    class ILogSinker
    {
    public:
        /**
         * @brief Logs a message with the specified log level.
         * 
         * @param level The log level of the message.
         * @param message The log message to be logged.
         */
        virtual void Log(const LogLevel level, const std::string& message) = 0;
        /**
         * @brief Virtual destructor to ensure proper cleanup in derived classes.
         */
        virtual ~ILogSinker() = default;
    };

    /**
     * @brief Singleton class for logging in the Concurrency library.
     * 
     * This class manages the logging process by registering log sinkers and
     * forwarding log messages to the registered sinker. It follows the singleton
     * pattern, ensuring that only one instance of the logger exists throughout
     * the application.
     */
    class ConcurrencyLog 
    {
    public:
        /**
         * @brief Registers a log sinker to handle log messages.
         * 
         * @param sinker A pointer to the log sinker to be registered.
         */
        void RegisterSinker(ILogSinker* sinker);
        /**
         * @brief Logs a message with the specified log level.
         * 
         * @param level The log level of the message.
         * @param message The log message to be logged.
         */
        void Log(const LogLevel level, const std::string& message);
        /**
         * @brief Destructor for the ConcurrencyLog class.
         */
        ~ConcurrencyLog();
        /**
         * @brief Retrieves the singleton instance of the ConcurrencyLog class.
         * 
         * @return A reference to the singleton instance of ConcurrencyLog.
         */
        static ConcurrencyLog& GetInstance();
    private:
        /**
         * @brief Private constructor to enforce the singleton pattern.
         */
        ConcurrencyLog();
        /**
         * @brief Pointer to the registered log sinker.
         */
        ILogSinker* sinker;
        /**
         * @brief Flag indicating whether the logger has been started.
         */
        bool started;
    };
} // namespace Concurrency