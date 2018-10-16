#ifndef HEADER_H
#define HEADER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <experimental/filesystem>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <fstream>
#include <future>
#include <thread>
#include <atomic>
#include <memory>
#include <queue>

using ULONG = unsigned long;
using ULONG64 = unsigned long long;
using BYTE = unsigned char;
using SOCKET = int;
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define GetError errno
#define CloseSocket(s) close(s)




namespace fs = std::experimental::filesystem;

namespace Test2
{

    namespace Constants
    {

        static const std::size_t FileNameLen = 0xFF, SizeDataBuf = 0x400 * 0x8, ErrorMessageLen = 0x80;

        static const std::string Ip = "localhost";

        static const int PortServer = 3333;

        static const int Port = 3334;

        static const int MaxConnections = 1;

    }

    namespace Protocol
    {

        enum CommandCode : BYTE
        {
            Ok,
            AcceptFile,
            Data,
            Error
        };

        struct Package
        {

            CommandCode m_enCode;

            union
            {

                struct
                {

                    char m_szNameFile[Constants::FileNameLen];

                    ULONG64 m_ul64SizeFile;

                }FileInfo;

                struct
                {

                    char m_bufData[Constants::SizeDataBuf];

                    std::size_t m_sizeBuf;

                }FileData;

                struct
                {

                    char m_bufMsg[Constants::ErrorMessageLen];

                }ErrorMessage;

            }m_body;

        };

    }

}

#endif // HEADER_H
