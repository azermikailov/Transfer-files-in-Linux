#ifndef RECEIVERWORKER_H
#define RECEIVERWORKER_H

#include "iworker.h"
#include "socket.h"
#include "header.h"

namespace Test2
{
    class ReceiverWorker : public IWorker
    {
        ReceiverWorker(const ReceiverWorker&) = delete;
        ReceiverWorker(ReceiverWorker&&) = delete;
        ReceiverWorker& operator = (const ReceiverWorker&) = delete;
        ReceiverWorker& operator = (ReceiverWorker&&) = delete;

        std::shared_ptr<Socket> m_socket{};

        fs::path m_pathFile{};

        ULONG64 m_ul64FileSize{};

        std::ofstream m_ofStream{};

        std::thread m_thread{};

        std::mutex m_mutex{};

        std::condition_variable m_cond_var{};

        std::queue<Protocol::Package> m_queuePackages{};

        std::atomic_bool m_atomStopFlag{};

        std::atomic_bool m_atomSuccessFlag{};

    public:

        explicit ReceiverWorker(std::shared_ptr<Socket>) noexcept;

        ~ReceiverWorker(void) noexcept;

        void DoWork(void) noexcept override;
       
    private:

        void ReceiveFileInfo(void);

        Protocol::Package ReceivePackage(void) const;

        void IsExistingFile(void) const;

        void IsNotFreeSpace(void) const;

        void OpenFile(void);

        void SendOkCommand(void) const;

        void SendPackage(const Protocol::Package&) const;

        void DownloadFile(void);

        void SendException(const std::exception&) const;

        void SaveDataToFile(void);

        void SavePackedToFile(void);

        void CloseFile(void);

        void SetSuccess(void);

        void Complete(void) noexcept;

    };

}
#endif // RECEIVERWORKER_H
