#ifndef IWORKER_H
#define IWORKER_H

#include "header.h"

namespace Test2
{
    class IWorker
    {
        IWorker(const IWorker&) = delete;
        IWorker(IWorker&&) = delete;
        IWorker& operator = (const IWorker&) = delete;
        IWorker& operator = (IWorker&&) = delete;

    protected:

        IWorker(void) = default;
          
    public:

        virtual ~IWorker(void) = default;

        template<typename T, typename... Args>
        static auto Create(Args&&... args)
        {
            return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
        }

        virtual void DoWork(void) noexcept = 0;

    };
}
#endif // IWORKER_H