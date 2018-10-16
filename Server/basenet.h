#ifndef BASENET_H
#define BASENET_H

#include "socket.h"
#include "header.h"

namespace Test2
{
    class BaseNet
    {
        BaseNet(void) = delete;
        BaseNet(const BaseNet&) = delete;
        BaseNet(BaseNet&&) = delete;
        BaseNet& operator = (const BaseNet&) = delete;
        BaseNet& operator = (BaseNet&&) = delete;

    protected:
  
        explicit BaseNet(std::shared_ptr<Socket>&,
            uint16_t&) noexcept;

        std::shared_ptr<Socket> m_socket;

        uint16_t m_uiPort;

        struct sockaddr_in m_sockAddIn;

    public:

        virtual ~BaseNet(void) = default;

    protected:

        void FillSockAddIn(void);
    };
}
#endif // BASENET_H