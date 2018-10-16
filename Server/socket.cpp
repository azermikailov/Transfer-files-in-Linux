#include "socket.h"

namespace Test2
{
    Socket::Socket(SOCKET socket) noexcept
        : m_Socket(socket)
    { }

    Socket::~Socket(void) noexcept
    {
        this->Close();
    }

    std::shared_ptr<Socket> Socket::Create(const int& af,
        const int& type,
        int protocol)
    {
        return Socket::CreateS(socket(af,
                                type,
                                protocol));
    }

    std::shared_ptr<Socket> Socket::CreateS(SOCKET socket)
    {
                      
        std::shared_ptr<Socket> sReturn(std::make_unique<Socket>(socket));

        if (!*sReturn)
            throw SocketException("Failed socket");

        return sReturn;
    }

    bool Socket::operator !(void) const noexcept
    {
        return this->m_Socket == 0 ||
            this->m_Socket == INVALID_SOCKET;
    }

    int Socket::Send(const char* pBuf,
        std::size_t sizeBuf) const
    {

        auto iReturn = send(this->m_Socket,
            pBuf,
            static_cast<int>(sizeBuf),
            {});

        if(iReturn == SOCKET_ERROR)
            throw SocketException("Failed send");

        return iReturn;
    }

    int Socket::Recv(char* pBuf,
        std::size_t sizeBuf) const
    {
        auto iReturn = recv(this->m_Socket,
            pBuf,
            static_cast<int>(sizeBuf),
            {});

        if (iReturn == SOCKET_ERROR)
            throw SocketException("Failed recv");

        return iReturn;
    }

    void Socket::Close(void) noexcept
    {
        if (!*this)
            close(this->m_Socket);

        this->m_Socket = {};
    }

    Socket::operator SOCKET(void) const noexcept
    {
        return this->m_Socket;
    }
}