#include "server.h"

namespace Test2
{
    Server::Server(std::shared_ptr<Socket> socket,
        uint16_t uiPort,
        int iBackLog) noexcept
        :
        BaseNet(socket,
            uiPort),
        m_iBackLog(iBackLog)
        { }

    Server::~Server(void) noexcept
    {
        this->Stop();
    }

    std::shared_ptr<Server> Server::Create(
        uint16_t uiPort,
        int backLog)
    {
        return std::shared_ptr<Server>(new Server(Socket::Create(),
            uiPort,
            backLog));
    }

    void Server::Start(void)
    {
        BaseNet::FillSockAddIn();

        this->Bind().Listen();
 
        this->m_thread = std::thread{ [this] { this->Accept(); } };
    }

    Server& Server::Bind(void)
    {
        if (::bind(*BaseNet::m_socket,
            reinterpret_cast<const sockaddr*>(&this->m_sockAddIn),
            sizeof(BaseNet::m_sockAddIn)) == SOCKET_ERROR)
            throw NetException("Failed bind");

        return *this;
    }

    Server& Server::Listen(void)
    {
        if (::listen(*BaseNet::m_socket,
            this->m_iBackLog) == SOCKET_ERROR)
            throw NetException("Failed listen");

        return *this;
    }

    void Server::Accept(void)
    {
        try
        {

            while (true)
            {
                socklen_t iAddrLen = sizeof(this->m_sockAddIn);

                this->m_atomStopFlag.store(false);                            

                auto socket = Socket::CreateS(::accept(*BaseNet::m_socket,
                    reinterpret_cast<struct sockaddr*>(&this->m_sockAddIn),
                    &iAddrLen));

                std::async(std::launch::async,
                    [this, socket] { ReceiveFile(socket); });
               
               this->m_atomStopFlag.store(true);
            }

        }
        catch (...) { }

    }

    void Server::ReceiveFile(std::shared_ptr<Socket> socket) noexcept
    {

        try
        {
            auto worker = IWorker::Create<ReceiverWorker>(socket);
            
            worker->DoWork();
        }
        catch (...) { }
    }

    void Server::Stop(void) noexcept
    {
        try
        {
            if (this->m_thread.joinable())
            {

                BaseNet::m_socket = {};

                if (m_atomStopFlag.load())
                {
                  this->m_thread.join();
                }
                else
                {
                  this->m_thread.detach();
                }

                this->m_thread = {};
            }

        }
        catch (...) { }
    }
}