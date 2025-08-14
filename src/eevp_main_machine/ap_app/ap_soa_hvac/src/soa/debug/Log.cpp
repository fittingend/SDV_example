#include <Common.h>
#include <iostream>
#include <streambuf>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <Log.hpp>
#if defined(USE_LOG_TELNET)
#include <TelnetServer.hpp>
#endif
#include <ara/log/logger.h>

static bool is_blank(const std::string& str)
{
	return std::all_of(str.begin(), str.end(), [](unsigned char c) {
		return std::isspace(c);
	});
}

static ara::log::Logger& logger()
{
	static ara::log::Logger& logger = ara::log::CreateLogger("SDST", "SDST");
	return logger;
}

static std::string logHeader(const char *path, const uint32_t line, const char *func)
{
	std::stringstream ss;
	std::string path_str(path);
	int find = (int)path_str.rfind('/') + 1;
	std::string file = path_str.substr(find, path_str.length() - find);

	ss << "[" << file << ":" << line << "|" << func << "] ";
	return ss.str();
}

soa::debug::Log &LOG_FATAL(const char *path, const uint32_t line, const char *func)
{
	static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_FATAL, "FATAL", true);
	std::string header = logHeader(path, line, func);
	log->setLogHeader(header);
	return *log;
}

soa::debug::Log &LOG_ERROR(const char *path, const uint32_t line, const char *func)
{
	static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_ERROR, "ERROR", true);
	std::string header = logHeader(path, line, func);
	log->setLogHeader(header);
	return *log;
}

soa::debug::Log &LOG_WARNING(const char *path, const uint32_t line, const char *func)
{
	static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_WARNING, "WARN", true);
	std::string header = logHeader(path, line, func);
	log->setLogHeader(header);
	return *log;
}

soa::debug::Log &LOG_INFO(const char *path, const uint32_t line, const char *func)
{
	static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_INFO, "INFO", true);
	std::string header = logHeader(path, line, func);
	log->setLogHeader(header);
	return *log;
}

soa::debug::Log &LOG_DEBUG(const char *path, const uint32_t line, const char *func)
{
	static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_DEBUG, "DEBUG", true);
	std::string header = logHeader(path, line, func);
	log->setLogHeader(header);
	return *log;
}

soa::debug::Log &LOG_VERBOSE(const char *path, const uint32_t line, const char *func)
{
	static soa::debug::Log *log = new soa::debug::Log(soa::debug::eLOG_VERBOSE, "VERBOSE", true);
	std::string header = logHeader(path, line, func);
	log->setLogHeader(header);
	return *log;
}

namespace soa
{
namespace debug
{
LogBuf::LogBuf(LogLevel_e level, std::string label, bool enable)
{
	this->mEnableLog = enable;
	this->mLevel = level;
	this->mLabel = label;
}

LogBuf::~LogBuf()
{
}

void LogBuf::flushString(const std::string& msg)
{
#if defined(USE_LOG_CONSOLE)
	if (this->mEnableLog)
	{
		switch (this->mLevel)
		{
		case soa::debug::eLOG_FATAL:
		case soa::debug::eLOG_ERROR:
		case soa::debug::eLOG_WARNING:
			std::cerr << "[" << this->mLabel << "] " << this->mLogHeader << msg << std::endl;
			break;
		case soa::debug::eLOG_INFO:
		case soa::debug::eLOG_DEBUG:
		case soa::debug::eLOG_VERBOSE:
		default:
			std::cout << "[" << this->mLabel << "] " << this->mLogHeader << msg << std::endl;
			break;
		}
	}
#endif

#if defined(USE_LOG_ARA_LOG)
	switch (this->mLevel)
	{
	case soa::debug::eLOG_FATAL:
		logger().LogFatal() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	case soa::debug::eLOG_ERROR:
		logger().LogError() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	case soa::debug::eLOG_WARNING:
		logger().LogWarn() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	case soa::debug::eLOG_INFO:
		logger().LogInfo() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	case soa::debug::eLOG_DEBUG:
		logger().LogDebug() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	case soa::debug::eLOG_VERBOSE:
		logger().LogVerbose() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	default:
		logger().LogDebug() << this->mLogHeader.c_str() << msg.c_str() << '\n';
		break;
	}
#endif

#if defined(USE_LOG_TELNET)
{
	std::stringstream telnet_string;
	telnet_string << "[" << this->mLabel << "] " << this->mLogHeader << msg << "\r\n";
	TelnetServer::GetInstance()->SendMessage(telnet_string.str());
}
#endif
}

int LogBuf::overflow(int c)
{
	if (c == '\n')
	{
		this->sync();
	}
	else if (c != EOF)
	{
		this->mStrBuffer.push_back(static_cast<char>(c));
	}

	return c;
}

int LogBuf::sync()
{
	if (!this->mStrBuffer.empty() && !is_blank(this->mStrBuffer))
	{
		flushString(this->mStrBuffer);
	}

	this->mStrBuffer.clear();
	return 0;
}

void LogBuf::setLogHeader(std::string header)
{
	this->mLogHeader = header;
}


Log::Log(LogLevel_e level, std::string label, bool enable)
{
	this->mEnableLog = enable;
	this->mLevel = level;
	this->mLabel = label;
	this->mLogBuffer = new LogBuf(level, label, enable);
	this->rdbuf(this->mLogBuffer);
}

Log::~Log()
{
	delete this->mLogBuffer;
}

void Log::setLogHeader(std::string header)
{
	this->mLogBuffer->setLogHeader(header);
}

}
}
