#include "senderworker.h"

namespace Test2
{
	CSenderWorker::CSenderWorker(std::shared_ptr<CClient> client,
		const std::string& strFile) noexcept
		:
		m_client(client),
		m_strFile(strFile)
	{  
	}

	CSenderWorker::~CSenderWorker(void) noexcept
	{

		this->m_client = {};

		if (this->m_threadReceive.joinable())
			this->m_threadReceive.join();

		if (this->m_threadSend.joinable())
			this->m_threadSend.join();

	}

	void CSenderWorker::DoWork(void) noexcept
	{

		try
		{
			this->IsNotFileExist();

			this->SaveFileInfo();

			this->OpenFile();

			this->ConnectToServer();

			this->SendFileInfo();

			this->ReceiveAnswer();

			this->CheckAnswer();

			this->ReadFileToQueue();

			this->m_threadReceive = std::thread{ [this] { ReceiveErrorMessage(); } };

			this->m_threadSend = std::thread{ [this] { SendFileFromQueue(); } };

			this->m_threadReceive.join();

			this->m_threadSend.join();

		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
                        
                        this->m_threadReceive.detach();
			
                        this->m_threadSend.detach(); 
		}
		catch (...)
		{
			std::cout << "Unhandled excpetion..." << std::endl;
		}

	}

	void CSenderWorker::IsNotFileExist(void) const
	{
		if (!fs::exists(this->m_strFile))
			throw FileException("File not existing");
	}

	void CSenderWorker::SaveFileInfo(void)
	{

		auto filename = fs::path(this->m_strFile).filename();

		this->m_package.m_enCode = Protocol::CommandCode::AcceptFile;

		::strcpy(this->m_package.m_body.FileInfo.m_szNameFile,
			filename.u8string().c_str());

		this->m_package.m_body.FileInfo.m_ul64SizeFile = fs::file_size(this->m_strFile);

	}

	void CSenderWorker::OpenFile(void)
	{

		this->m_ifStream.open(this->m_strFile,
			std::ios::binary);
		if (!this->m_ifStream.is_open())
			throw FileException("Error open file");

	}

	void CSenderWorker::ConnectToServer(void) const
	{
		this->m_client->Connect();
	}

	void CSenderWorker::SendFileInfo(void)
	{
        	this->SendPackage(this->m_package);
	}

	void CSenderWorker::SendPackage(const Protocol::Package& package) const
	{
		this->m_client->Send(reinterpret_cast<const char*>(&package),
			sizeof(package));
	}

	void CSenderWorker::ReceiveAnswer(void)
	{
		this->m_package = this->ReceivePackage();
	}

	Protocol::Package CSenderWorker::ReceivePackage(void)
	{

		Protocol::Package package{};

		this->m_client->Recv(reinterpret_cast<char*>(&package),
			sizeof(package));

		return package;
	}

	void CSenderWorker::CheckAnswer(void)
	{
		if(this->m_package.m_enCode != Protocol::CommandCode::Ok || 
			this->m_package.m_enCode == Protocol::CommandCode::Error)
			throw FileException(this->m_package.m_body.ErrorMessage.m_bufMsg);
	}

	void CSenderWorker::ReceiveErrorMessage(void)
	{
		try
		{
			auto package = this->ReceivePackage();

			switch (package.m_enCode)
			{

			case Protocol::CommandCode::Error:
				std::cout << package.m_body.ErrorMessage.m_bufMsg << std::endl;
				this->m_atomStopFlag = true;
				break;
				
				case Protocol::CommandCode::Ok:
					std::cout << "Ok" << std::endl;
					break;
	
			}
		}
		catch (...) { }

	}

	void CSenderWorker::ReadFileToQueue(void)
	{
		while (!this->m_ifStream.eof() && !this->m_atomStopFlag)
		{

			Protocol::Package package{};
			package.m_enCode = Protocol::CommandCode::Data;

			auto countRead = this->m_ifStream.read(package.m_body.FileData.m_bufData,
				sizeof(package.m_body.FileData.m_bufData)).gcount();
			if (!countRead)
				break;

			package.m_body.FileData.m_sizeBuf = static_cast<std::size_t>(countRead);

			std::unique_lock<std::mutex> lock(this->m_mutex);

			this->m_queuePackages.push(package);

			this->m_cond_var.notify_one();
				
		}

		this->m_atomSuccessFlag = true;
	}

	void CSenderWorker::SendFileFromQueue(void)
	{

		try
		{
			while (true)
			{

				std::unique_lock<std::mutex> lock(this->m_mutex);

				this->m_cond_var.wait(lock,
					[this] { return m_atomStopFlag || m_atomSuccessFlag || !m_queuePackages.empty(); });

				if (this->m_atomStopFlag)
					return;

				if (this->m_atomSuccessFlag)
				{

					while (!this->m_queuePackages.empty())
						this->PopPackageAndSend();

					return;

				}

				this->PopPackageAndSend();

			}

			return;

		}
		catch (...) {}

		this->m_atomStopFlag = true;

	}

	void CSenderWorker::PopPackageAndSend(void)
	{
		auto package = this->m_queuePackages.front();

		this->m_queuePackages.pop();

		this->SendPackage(package);
	}
}
