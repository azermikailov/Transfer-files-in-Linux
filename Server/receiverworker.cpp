#include "receiverworker.h"

namespace Test2
{
    ReceiverWorker::ReceiverWorker(std::shared_ptr<Socket> socket) noexcept
            :m_socket(socket)
           { }    


    ReceiverWorker::~ReceiverWorker(void) noexcept
    {
        this->Complete();
    }

    void ReceiverWorker::DoWork(void) noexcept
    {

        try
        {

            this->ReceiveFileInfo();

            this->IsExistingFile();

            this->IsNotFreeSpace();

            this->OpenFile();

            this->m_thread = std::thread{ [this] { SaveDataToFile(); } };

            this->SendOkCommand();

            this->DownloadFile();

            this->SetSuccess();

            this->m_thread.join();

            this->CloseFile();

            this->SendOkCommand();

            return;

        }
        catch (const NetException&)
        { }
        catch (const std::exception& ex)
        {
            this->SendException(ex);
        }
        catch (...)
        { }

        this->m_atomStopFlag = true;
    }

    void ReceiverWorker::ReceiveFileInfo(void)
    {

        auto package = this->ReceivePackage();
        if (package.m_enCode != Protocol::CommandCode::AcceptFile)
            throw std::runtime_error("Error CommandCode");

        this->m_ul64FileSize = package.m_body.FileInfo.m_ul64SizeFile;

        this->m_pathFile =fs::current_path() / package.m_body.FileInfo.m_szNameFile;

    }

    Protocol::Package ReceiverWorker::ReceivePackage(void) const
    {

        Protocol::Package package{};

        this->m_socket->Recv(reinterpret_cast<char*>(&package),
            sizeof(package));

        return package;
    }

    void ReceiverWorker::IsExistingFile(void) const
    {

        if(fs::exists(this->m_pathFile))
            throw FileException("File exising");

    }

    void ReceiverWorker::IsNotFreeSpace(void) const
    {
        if (fs::space(fs::current_path()).available < this->m_ul64FileSize)
            throw FileException("Not free space");

    }

    void ReceiverWorker::OpenFile(void)
    {
        this->m_ofStream.open(this->m_pathFile,
            std::ios::binary);
        if (!this->m_ofStream.is_open())
            throw FileException("Failed open file");

    }

    void ReceiverWorker::SendOkCommand(void) const
    {
        Protocol::Package package{};
        package.m_enCode = Protocol::Ok;

        this->SendPackage(package);
    }

    void ReceiverWorker::SendPackage(const Protocol::Package& package) const
    {
        this->m_socket->Send(reinterpret_cast<const char*>(&package),
            sizeof(package));
    }

    void ReceiverWorker::DownloadFile(void)
    {
        ULONG64 ul64Count{};

        while (ul64Count < this->m_ul64FileSize && !this->m_atomStopFlag)
        {

            auto package = this->ReceivePackage();
            if (package.m_enCode != Protocol::CommandCode::Data)
                throw std::invalid_argument("Invalid command code");

            ul64Count += package.m_body.FileData.m_sizeBuf;

            std::unique_lock<std::mutex> lock(this->m_mutex);

            this->m_queuePackages.push(package);

            this->m_cond_var.notify_one();
        }

    }

    void ReceiverWorker::SendException(const std::exception& ex) const
    {

        try
        {

            Protocol::Package package{};
            package.m_enCode = Protocol::Error;

            std::string exError(ex.what());
            std::copy(exError.cbegin(),
                     exError.cend(),
                      &package.m_body.ErrorMessage.m_bufMsg[0]);

            this->SendPackage(package);

        }
        catch (...)
        { }

    }

    void ReceiverWorker::SaveDataToFile(void)
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
                        this->SavePackedToFile();

                    return;

                }

                this->SavePackedToFile();

            }

            return;

        }
        catch (const std::exception& ex)
        {
            this->SendException(ex);
        }
        catch (...)
        { }

        this->m_atomStopFlag = true;
    }

    void ReceiverWorker::SavePackedToFile(void)
    {

        auto package = this->m_queuePackages.front();

        this->m_queuePackages.pop();

        if (this->m_ofStream.write(package.m_body.FileData.m_bufData,
            package.m_body.FileData.m_sizeBuf).fail())
            throw FileException("Error write data to file");

    }

    void ReceiverWorker::CloseFile(void)
    {
        this->m_ofStream.close();
    }

    void ReceiverWorker::SetSuccess(void)
    {
        this->m_atomSuccessFlag = true;
    }

    void ReceiverWorker::Complete(void) noexcept
    {
        try
        {
            if (this->m_atomSuccessFlag)
                return;

            if (this->m_thread.joinable())
                this->m_thread.join();

            if (this->m_ofStream.is_open())
            {
                this->m_ofStream.close();

               fs::remove(this->m_pathFile);
            }
       }
       catch (...)
       { }
    }
}
