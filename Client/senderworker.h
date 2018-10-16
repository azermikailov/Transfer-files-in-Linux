#include "iworker.h"
#include "client.h"

namespace Test2
{

	class CSenderWorker : public IWorker
	{

		CSenderWorker(const CSenderWorker&) = delete;
		CSenderWorker(CSenderWorker&&) = delete;
		CSenderWorker& operator = (const CSenderWorker&) = delete;
		CSenderWorker& operator = (CSenderWorker&&) = delete;

		std::shared_ptr<CClient> m_client{};

		std::string m_strFile{};

		std::ifstream m_ifStream{};

		std::thread m_threadReceive{};

		std::atomic_bool m_atomStopFlag{};

		std::atomic_bool m_atomSuccessFlag{};

		Protocol::Package m_package{};

		std::queue<Protocol::Package> m_queuePackages{};

		std::mutex m_mutex{};

		std::condition_variable m_cond_var{};

		std::thread m_threadSend{};

	public:

		explicit CSenderWorker(std::shared_ptr<CClient>,
			const std::string&) noexcept;

		~CSenderWorker(void) noexcept;

		void DoWork(void) noexcept override;

	private:

		void IsNotFileExist(void) const;

		void SaveFileInfo(void);

		void OpenFile(void);

		void ConnectToServer(void) const;

		void SendFileInfo(void);

		void SendPackage(const Protocol::Package&) const;

		void ReceiveAnswer(void);

		Protocol::Package ReceivePackage(void);

		void CheckAnswer(void);

		void ReceiveErrorMessage(void);

		void ReadFileToQueue(void);

		void SendFileFromQueue(void);

		void PopPackageAndSend(void);

	};

}
