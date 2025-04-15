
#pragma once
#include <chrono>

namespace Concurrency
{
    using TaskPriority = int32_t;
    class ITask
    {
    public:
        virtual ~ITask() = default;
        virtual bool Run() = 0;
    };
    class Thread;

} // namespace Concurrency
