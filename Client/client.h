#include "basenet.h"

#ifndef CLIENT_H
#define CLIENT_H

namespace Test2
{
	class CClient : CBaseNet
	{

		CClient(void) = delete;
		CClient(const CClient&) = delete;
		CClient(CClient&&) = delete;
		CClient& operator = (const CClient&) = delete;
		CClient& operator = (CClient&&) = delete;

		explicit CClient(std::shared_ptr<CSocket>,
			const std::string&,
			uint16_t) noexcept;

	public:

		~CClient(void) noexcept;

		static std::shared_ptr<CClient> Create(const std::string& = Constants::Ip,
			uint16_t = Constants::Port);

		void Bind(void);
		
		void Connect(void);

		int Send(const char*,
			std::size_t) const;

		int Recv(char*,
			std::size_t) const;

		void Close(void) noexcept;

		std::shared_ptr<CSocket> Socket(void) const noexcept;

	};
}
#endif //CLIENT_H
