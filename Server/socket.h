#ifndef CSOCKET_H
#define CSOCKET_H

#include "exceptions.h"
#include "header.h"

namespace Test2
{
    class Socket
    {
        Socket(const Socket&) = delete;
        Socket(Socket&&) = delete;
        Socket& operator = (const Socket&) = delete;
        Socket& operator = (Socket&&) = delete;

        SOCKET m_Socket{};

    public:

        explicit Socket(SOCKET = {}) noexcept;

        ~Socket(void) noexcept;

        static std::shared_ptr<Socket> Create (const int& af = AF_INET,
            const int& type = SOCK_STREAM,
            int protocol = 0);

        static std::shared_ptr<Socket> CreateS(SOCKET);

        bool operator !(void) const noexcept;

        int Send(const char*,
            std::size_t) const;

        int Recv(char*,
            std::size_t) const;

        void Close(void) noexcept;

        operator SOCKET(void) const noexcept;
    };
}
#endif // CSOCKET_H