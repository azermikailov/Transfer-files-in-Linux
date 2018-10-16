#include "csocket.h"

namespace Test2
{
    CSocket::CSocket(SOCKET socket) noexcept
        : m_Socket(socket)
    { }

    CSocket::~CSocket(void) noexcept
    {
        this->Close();
    }

    std::shared_ptr<CSocket> CSocket::Create(int af,
        int type,
        int protocol)
    {
        return CSocket::CreateS(socket(af,
                                type,
                                protocol));
    }

    std::shared_ptr<CSocket> CSocket::CreateS(SOCKET socket)
    {
        std::shared_ptr<CSocket> sReturn(std::make_unique<CSocket>(socket));

        if (!*sReturn)
            throw SocketException("Failed socket",
                GetError);

        return sReturn;

    }

    bool CSocket::operator !(void) const noexcept
    {
        return this->m_Socket == 0 ||
            this->m_Socket == INVALID_SOCKET;
    }

    int CSocket::Send(const char* pBuf,
        std::size_t sizeBuf) const
    {

        auto iReturn = send(this->m_Socket,
            pBuf,
            static_cast<int>(sizeBuf),
            {});

        if(iReturn == SOCKET_ERROR)
            throw SocketException("Failed send",
                GetError);

        return iReturn;

    }

    int CSocket::Recv(char* pBuf,
        std::size_t sizeBuf) const
    {
          auto iReturn = recv(this->m_Socket,
            pBuf,
            static_cast<int>(sizeBuf),
            {});

        if (iReturn == SOCKET_ERROR)
            throw SocketException("Failed recv",
                GetError);

        return iReturn;
    }

    void CSocket::Close(void) noexcept
    {
        if (!*this)
            ::close(this->m_Socket);

        this->m_Socket = {};
    }

    CSocket::operator SOCKET(void) const noexcept
    {
        return this->m_Socket;
    }
}
