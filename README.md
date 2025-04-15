# Concurrency

This repository, named `Concurrency`, primarily offers a scheduler for concurrent operations, which can assist developers in more conveniently managing and executing concurrent tasks.

## Directory Structure

- `LICENSE`: Contains information about the project's open - source license.
- `README.md`: The project's documentation, briefly introducing that the project is a concurrent scheduler.
- `x64-win/`: Code and libraries related to the 64 - bit platform.
  - `include/`: Contains header files for the 64 - bit platform.
  - `lib/`: May contain library files for the 64 - bit platform.
- `x86-win/`: Code and libraries related to the 32 - bit Windows platform.
  - `include/`: Contains header files for the 32 - bit Windows platform.
  - `lib/`: May contain library files for the 32 - bit Windows platform.

## Main Classes and Interfaces

### 1. `IScheduledWorker`
- **Definition**: Defined in `Concurrency/x64-win/include/Concurrency/IScheduledWorker.hpp` and `Concurrency/x86-win/include/Concurrency/IScheduledWorker.hpp`.
- **Function**: This is an abstract base class that defines the basic interface of a schedulable worker. It includes `RunOnce()` for executing a single task, `GetWorkerName()` for getting the name of the worker, and `NotifyDurationTimeout()` for notifying timeout events.
- **Derived Class**: `ScheduledWorkerAgent` is a concrete implementation class of `IScheduledWorker`. It can take a task action (`Action`) and a timeout callback (`TimeoutCallback`) through its constructor.

### 2. `IScheduler`
- **Definition**: Defined in `Concurrency/x64-win/include/Concurrency/IScheduler.hpp` and `Concurrency/x86-win/include/Concurrency/IScheduler.hpp`.
- **Function**: This is an abstract base class that defines the basic interface of a scheduler. It includes multiple `Attach()` methods for adding a schedulable worker (`IScheduledWorker`) or a task action (`Action`) to the scheduler, and specifying parameters such as execution interval, thread priority, and timeout.

### 3. `ITask`
- **Definition**: Defined in `Concurrency/x64-win/include/Concurrency/ITask.hpp`.
- **Function**: This is an abstract base class that defines the basic interface of a runnable object, including the `Run()` method for executing specific tasks.

### 4. `CyclicalWorker`
- **Definition**: Defined in `Concurrency/x64-win/include/Concurrency/CyclicalWorker.hpp` respectively.
- **Function**: Both classes inherit from `ITask` and are used to implement cyclical workers and critical workers. They can execute cyclical tasks and handle execution errors and timeout events.

### 5. `ConcurrencyLog`
- **Definition**: Defined in `Concurrency/x64-win/include/Concurrency/ConcurrencyLog.hpp` and `Concurrency/x86-win/include/Concurrency/ConcurrencyLog.hpp`.
- **Function**: Defines the log level enumeration (`LogLevel`) and the log sinker interface (`ILogSinker`) for recording and handling log information.

## Usage Example

### 1. `Scheduler`
The following is a simple example demonstrating how to use the `Scheduler`.

```cpp
#include <iostream>
#include <Concurrency/Scheduler.hpp>
#include <Concurrency/IScheduledWorker.hpp>

// Implement a simple IScheduledWorker class
class MyWorker : public Concurrency::IScheduledWorker {
public:
    MyWorker(const char* name) : name_(name) {}

    void RunOnce() override {
        std::cout << "Worker " << name_ << " is running." << std::endl;
    }

    const char* GetWorkerName() const override {
        return name_.c_str();
    }

    void NotifyDurationTimeout(const bool& isTimeout) const override {
        if (isTimeout) {
            std::cout << "Worker " << name_ << " has timed out." << std::endl;
        }
    }

private:
    std::string name_;
};

int main() {
    // Create a scheduler instance
    Concurrency::Scheduler scheduler(50);

    // Create a work item instance
    MyWorker worker("MyWorker");

    // Set the execution interval of the work item to 1000 milliseconds
    Concurrency::Millisecond interval = 1000;

    // Add the work item to the scheduler
    scheduler.Attach(worker, interval, 50);

    // Activate the scheduler
    scheduler.Activate();

    // Let the main thread sleep for a while to observe the execution of the work item
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Deactivate the scheduler
    scheduler.Deactivate();

    return 0;
}
```
#### Code Explanation

1.  **MyWorker Class**: Implements the IScheduledWorker interface, defining the specific behavior of the work item. The RunOnce method defines the execution logic of the work item, the GetWorkerName method returns the name of the work item, and the NotifyDurationTimeout method handles timeout notifications.
    
2.  **main Function**:
    
    *   Creates a Scheduler instance, specifying the priority of the worker thread as Normal.
        
    *   Creates a MyWorker instance as the specific work item.
        
    *   Calls the scheduler.Attach method to add the work item to the scheduler, specifying an execution interval of 1000 milliseconds.
        
    *   Calls the scheduler.Activate method to activate the scheduler and start executing the work item.
        
    *   The main thread sleeps for 5 seconds to observe the execution of the work item.
        
    *   Calls the scheduler.Deactivate method to deactivate the scheduler and stop executing the work item.
	
Through this example, you can see how to use the Scheduler, CyclicalWorker, and Thread classes to implement a simple concurrent scheduling system.
        
### 2. `ConcurrencyLog`


```cpp
#include <iostream>
#include <Concurrency/ConcurrencyLog.hpp>

// Implement a simple ILogSinker class
class ConsoleLogSinker : public Concurrency::ILogSinker {
public:
    void Log(const Concurrency::LogLevel level, const std::string& message) override {
        switch (level) {
            case Concurrency::LogLevel::Trace:
                std::cout << "[Trace] ";
                break;
            case Concurrency::LogLevel::Debug:
                std::cout << "[Debug] ";
                break;
            case Concurrency::LogLevel::Info:
                std::cout << "[Info] ";
                break;
            case Concurrency::LogLevel::Warning:
                std::cout << "[Warning] ";
                break;
            case Concurrency::LogLevel::Error:
                std::cout << "[Error] ";
                break;
        }
        std::cout << message << std::endl;
    }
};

int main() {
    // Get the singleton instance of ConcurrencyLog
    Concurrency::ConcurrencyLog& logger = Concurrency::ConcurrencyLog::GetInstance();

    // Create a log sink instance
    ConsoleLogSinker sinker;

    // Register the log sink
    logger.RegisterSinker(&sinker);

    // Log messages at different levels
    logger.Log(Concurrency::LogLevel::Trace, "This is a trace message.");
    logger.Log(Concurrency::LogLevel::Debug, "This is a debug message.");
    logger.Log(Concurrency::LogLevel::Info, "This is an info message.");
    logger.Log(Concurrency::LogLevel::Warning, "This is a warning message.");
    logger.Log(Concurrency::LogLevel::Error, "This is an error message.");

    return 0;
}
```
#### Code Explanation

1. `ConsoleLogSinker` Class

    *   Implements the Concurrency::ILogSinker interface.
	
    *   The Log method adds a prefix based on the log level and outputs the log message to the console.

2. main Function

    *   Getting the Logger Instance: Obtains the singleton instance of ConcurrencyLog using Concurrency::ConcurrencyLog::GetInstance().
	
    *   Creating a Log Sink: Creates an instance of ConsoleLogSinker.
	
    *   Registering the Log Sink: Registers the ConsoleLogSinker with the ConcurrencyLog instance using RegisterSinker.
	
    *   Logging Messages: Logs messages at different levels using the Log method of the ConcurrencyLog instance.

## Summary

This repository provides a complete concurrent scheduler framework, including core components such as schedulable workers, schedulers, threads, and logs. Developers can use these interfaces and classes to implement their own concurrent task scheduling and management systems.