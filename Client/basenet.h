#ifndef BASENET_H
#define BASENET_H

#include "csocket.h"
#include "header.h"

namespace Test2
{
    class CBaseNet
    {
        CBaseNet(void) = delete;
        CBaseNet(const CBaseNet&) = delete;
        CBaseNet(CBaseNet&&) = delete;
        CBaseNet& operator = (const CBaseNet&) = delete;
        CBaseNet& operator = (CBaseNet&&) = delete;

    protected:

        explicit CBaseNet(std::shared_ptr<CSocket>,
            const std::string&,
            uint16_t) noexcept;

        std::shared_ptr<CSocket> m_socket{};

        std::string m_strIp{};

        uint16_t m_uiPort{};

        struct  sockaddr_in m_sockAddIn{};
        struct  sockaddr_in m_sockServer{};
    public:

        virtual ~CBaseNet(void) = default;

    protected:

        void FillSockAddIn(void);

        void FillSockServer(void);
    };
}
#endif // BASENET_H