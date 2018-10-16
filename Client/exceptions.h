#include "header.h"

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

namespace Test2
{
    class Exception : public std::runtime_error
    {

        int m_iCode{};

    public:

        Exception(void) = delete;
        Exception(const Exception&) = default;
        Exception(Exception&&) = default;
        Exception& operator = (const Exception&) = default;
        Exception& operator = (Exception&&) = default;
        ~Exception(void) = default;

        Exception(const std::string& strMsg,
            int iCode = {})
            :
           std::runtime_error(strMsg.c_str()),
            m_iCode(iCode)
        { }

        int GetCode(void) const noexcept
        {
            return this->m_iCode;
        }

    };

    class NetException : public Exception
    {

    public:

        using Exception::Exception;

    };

    class SocketException : public NetException
    {

    public:

        using NetException::NetException;

    };

    class FileException : public Exception
    {

    public:

        using Exception::Exception;

    };
}
#endif // EXCEPTIONS_H
