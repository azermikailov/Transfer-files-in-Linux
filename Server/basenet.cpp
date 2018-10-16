#include "basenet.h"

namespace Test2
{
    BaseNet::BaseNet(std::shared_ptr<Socket>&socket,
        uint16_t& uiPort) noexcept
        :
        m_socket(socket),
        m_uiPort(uiPort),
        m_sockAddIn({})
    { }

    void BaseNet::FillSockAddIn(void)
    {
        this->m_sockAddIn = {};
        memset(&m_sockAddIn,0,sizeof(struct sockaddr_in));         
 
        this->m_sockAddIn.sin_family = AF_INET;
        this->m_sockAddIn.sin_addr.s_addr = INADDR_ANY;
        this->m_sockAddIn.sin_port = this->m_uiPort;

        if (this->m_sockAddIn.sin_addr.s_addr == INADDR_NONE)
            throw NetException("Failed inet_addr");
    }
}
