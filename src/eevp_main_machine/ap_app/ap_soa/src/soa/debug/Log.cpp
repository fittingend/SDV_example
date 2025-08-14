#include <Common.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <ara/log/logger.h>
#include <Log.hpp>

std::vector<std::string> split(std::string str, char Delimiter) {
    std::istringstream iss(str);
    std::string buffer;

    std::vector<std::string> result;

    while (getline(iss, buffer, Delimiter))
    {
        result.push_back(buffer);
    }

    char lastChar = str[str.size() - 1];
    if (lastChar == '\n')
    {
        result.push_back("");
    }

    return result;
}

static ara::log::Logger& logger()
{
    static ara::log::Logger& logger = ara::log::CreateLogger("SOA", "Default");
    return logger;
}




soa::debug::Log &LOG_FATAL()
{
    static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_FATAL, "FATAL", true);
    return *log;
}

extern soa::debug::Log &LOG_ERROR()
{
    static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_ERROR, "ERROR", true);
    return *log;
}

extern soa::debug::Log &LOG_WARNING()
{
    static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_WARNING, "WARN", true);
    return *log;
}

extern soa::debug::Log &LOG_INFO()
{
    static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_INFO, "INFO", true);
    return *log;
}

extern soa::debug::Log &LOG_DEBUG()
{
    static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_DEBUG, "DEBUG", true);
    return *log;
}

extern soa::debug::Log &LOG_VERBOSE()
{
    static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_VERBOSE, "VERBOSE", true);
    return *log;
}

namespace soa
{
namespace debug
{
Log::Log(soa::debug::LogLevel_e level, std::string label, bool enable)
{
    this->mEnableLog = enable;
    this->mLevel = level;
    this->mLabel = label;
    this->mStream.str("");
}

Log::~Log()
{

}

Log & Log::operator<<(bool value)
{
    this->mStream << ((value) ? "true" : "false");
    return *this;
}

Log & Log::operator<<(char value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(unsigned char value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(short value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(unsigned short value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(int value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(unsigned int value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(long value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(unsigned long value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(float value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(double value)
{
    this->mStream << value;
    return *this;
}

Log & Log::operator<<(const char *str)
{
    if (str)
    {
        int src, dst;
        char temp[1024];

        for (src = 0, dst = 0; str[src] != '\0'; src++)
        {
            if (str[src] == '\n')
            {
                temp[dst] = '\0';
                this->mStream << temp;
                dst = 0;
                this->flush();
            }
            else
            {
                temp[dst] = str[src];
                dst++;
                if (dst >= 1020)
                {
                    temp[dst] = '\0';
                    this->mStream << temp;
                    dst = 0;
                }
            }
        }

        if (dst > 0)
        {
            temp[dst] = '\0';
            this->mStream << temp;
        }
    }

    return *this;
}

Log & Log::operator<<(std::string &str)
{
    std::vector<std::string> strVector = split(str, '\n');
    int length = strVector.size();
    int i = 0;
    for (std::string strItem: strVector) {
        if (strItem != "")
        {
            this->mStream << strItem;
        }

        i++;
        if (i < length) {
            this->flush();
        }
    }

    return *this;
}

void Log::flush()
{
    std::string string = this->mStream.str();

#if defined(USE_LOG_CONSOLE)
    if (this->mEnableLog)
    {
        switch (this->mLevel)
        {
        case soa::debug::eLOG_FATAL:
        case soa::debug::eLOG_ERROR:
        case soa::debug::eLOG_WARNING:
            std::cerr << "[" << this->mLabel << "] " << string << '\n';
            break;
        case soa::debug::eLOG_INFO:
        case soa::debug::eLOG_DEBUG:
        case soa::debug::eLOG_VERBOSE:
        default:
            std::cout << "[" << this->mLabel << "] " << string << '\n';
            break;
        }
    }

#endif

#if defined(USL_LOG_ARA_LOG)
        switch (this->mLevel)
        {
        case soa::debug::eLOG_FATAL:
            logger().LogFatal() << string.c_str();
            break;
        case soa::debug::eLOG_ERROR:
            logger().LogError() << string.c_str();
            break;
        case soa::debug::eLOG_WARNING:
            logger().LogWarn() << string.c_str();
            break;
        case soa::debug::eLOG_INFO:
            logger().LogInfo() << string.c_str();
            break;
        case soa::debug::eLOG_DEBUG:
            logger().LogDebug() << string.c_str();
            break;
        case soa::debug::eLOG_VERBOSE:
            logger().LogVerbose() << string.c_str();
            break;
        default:
            logger().LogDebug() << string.c_str();
            break;
        }
#endif

    // Clear stream
    this->mStream.str("");
}

}
}