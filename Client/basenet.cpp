#include "basenet.h"

namespace Test2
{
    CBaseNet::CBaseNet(std::shared_ptr<CSocket> socket,
        const std::string& strIp,
        uint16_t uiPort) noexcept
        :
        m_socket(socket),
        m_strIp(strIp),
        m_uiPort(uiPort)
    { }

    void CBaseNet::FillSockAddIn(void)
    {
        this->m_sockAddIn = {};
        memset(&m_sockAddIn,0,sizeof(struct sockaddr_in));         
 
        this->m_sockAddIn.sin_family = AF_INET;
        this->m_sockAddIn.sin_addr.s_addr = INADDR_ANY;
        this->m_sockAddIn.sin_port = this->m_uiPort;

        if (this->m_sockAddIn.sin_addr.s_addr == INADDR_NONE)
            throw NetException("Failed inet_addr");
    }

    void CBaseNet::FillSockServer(void)
    {
        this->m_sockServer ={};
        memset(&m_sockServer,0,sizeof(struct sockaddr_in));         
        struct hostent *h = gethostbyname (this->m_strIp.c_str());
        
        this->m_sockServer.sin_family = AF_INET;
                                   
        memcpy ((char *)&this->m_sockServer.sin_addr,h->h_addr,h->h_length);
                                   
        this->m_sockServer.sin_port = Constants::PortServer;
    }
}
