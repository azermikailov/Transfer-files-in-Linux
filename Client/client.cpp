
#include "client.h"

namespace Test2
{

	CClient::CClient(std::shared_ptr<CSocket> socket,
		const std::string& strIp,
		uint16_t uiPort) noexcept
		:
		CBaseNet(socket,
			strIp,
			uiPort)
	{ }

	CClient::~CClient(void) noexcept
	{ }

	std::shared_ptr<CClient> CClient::Create(const std::string& strIp,
		uint16_t uiPort)
	{
		return std::shared_ptr<CClient>(new CClient(CSocket::Create(),
			strIp,
			uiPort));
	}

	void CClient::Connect(void)
	{
		CBaseNet::FillSockAddIn();

		this->Bind(); 

        CBaseNet::FillSockServer();

		if (connect(*CBaseNet::m_socket,
			reinterpret_cast<const sockaddr*>(&this->m_sockServer),
			sizeof(CBaseNet::m_sockServer)) == SOCKET_ERROR)
			throw NetException("Failed connect");
	}

    void CClient::Bind(void)
    {
        if (::bind(*CBaseNet::m_socket,
            reinterpret_cast<const sockaddr*>(&this->m_sockAddIn),
            sizeof(CBaseNet::m_sockAddIn)) == SOCKET_ERROR)
            throw NetException("Failed bind",
                GetError);

    }

	int CClient::Send(const char* pBuf,
		std::size_t sizeBuf) const
	{
		return CBaseNet::m_socket->Send(pBuf,
			sizeBuf);
	}

	int CClient::Recv(char* pBuf,
		std::size_t sizeBuf) const
	{
		return CBaseNet::m_socket->Recv(pBuf,
			sizeBuf);
	}

	void CClient::Close(void) noexcept
	{
		CBaseNet::m_socket->Close();
	}

	std::shared_ptr<CSocket> CClient::Socket(void) const noexcept
	{
		return CBaseNet::m_socket;
	}
}
