#include "exceptions.h"
#include "header.h"

#ifndef CSOCKET_H
#define CSOCKET_H

namespace Test2
{
    class CSocket
    {
        CSocket(const CSocket&) = delete;
        CSocket(CSocket&&) = delete;
        CSocket& operator = (const CSocket&) = delete;
        CSocket& operator = (CSocket&&) = delete;

        SOCKET m_Socket{};

    public:

        explicit CSocket(SOCKET = {}) noexcept;

        ~CSocket(void) noexcept;

        static std::shared_ptr<CSocket> Create (int = AF_INET,
            int type = SOCK_STREAM,
            int protocol = 0);

        static std::shared_ptr<CSocket> CreateS(SOCKET);

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
