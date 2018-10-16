#ifndef CSERVER_H
#define CSERVER_H

#include "iworker.h"
#include "basenet.h"
#include "receiverworker.h"

namespace Test2
{
    class Server : BaseNet
    {
        Server(void) = delete;
        Server(const Server&) = delete;
        Server(Server&&) = delete;
        Server& operator = (const Server&) = delete;
        Server& operator = (Server&&) = delete;

        int m_iBackLog{};

        std::thread m_thread{};

        std::atomic_bool m_atomStopFlag{};

        explicit Server(std::shared_ptr<Socket>,
            uint16_t,
            int) noexcept;
    public:

        ~Server(void) noexcept;

        static std::shared_ptr<Server> Create(uint16_t = Constants::Port,
            int = Constants::MaxConnections);

    public:

        void Start(void);

    private:

        Server & Bind(void);

        Server& Listen(void);

        void Accept(void);

        void ReceiveFile(std::shared_ptr<Socket>) noexcept;

    public:

        void Stop(void) noexcept;

    };
}
#endif // CSERVER_H