#ifndef ___DEBUG_LOG_HPP___
#define ___DEBUG_LOG_HPP___

#include <string>
#include <sstream>
#include <streambuf>

namespace soa
{
namespace debug
{
typedef enum
{
	eLOG_FATAL = 0x01,
	eLOG_ERROR = 0x02,
	eLOG_WARNING = 0x04,
	eLOG_INFO = 0x08,
	eLOG_DEBUG = 0x10,
	eLOG_VERBOSE = 0x20,

	eLOG_ALL = 0xFF
} LogLevel_e;

class LogBuf : public std::streambuf {
private:
	bool mEnableLog;
	LogLevel_e mLevel;
	std::string mLabel;
	std::string mLogHeader;
	std::string mStrBuffer;

	void flushString(const std::string& msg);

protected:
	int overflow(int c) override;
	int sync() override;

public:
	LogBuf(LogLevel_e level, std::string label, bool enable = false);
	~LogBuf();
	void setLogHeader(std::string header);
};

class Log : public std::ostream {
private:
	bool mEnableLog;
	LogLevel_e mLevel;
	std::string mLabel;
	LogBuf *mLogBuffer;

public:
	Log(LogLevel_e level, std::string label, bool enable = false);
	~Log();
	void setLogHeader(std::string header);
};

}
}

extern soa::debug::Log &LOG_FATAL(const char *path = __builtin_FILE(), const uint32_t line = __builtin_LINE(), const char *func = __builtin_FUNCTION());
extern soa::debug::Log &LOG_ERROR(const char *path = __builtin_FILE(), const uint32_t line = __builtin_LINE(), const char *func = __builtin_FUNCTION());
extern soa::debug::Log &LOG_WARNING(const char *path = __builtin_FILE(), const uint32_t line = __builtin_LINE(), const char *func = __builtin_FUNCTION());
extern soa::debug::Log &LOG_INFO(const char *path = __builtin_FILE(), const uint32_t line = __builtin_LINE(), const char *func = __builtin_FUNCTION());
extern soa::debug::Log &LOG_DEBUG(const char *path = __builtin_FILE(), const uint32_t line = __builtin_LINE(), const char *func = __builtin_FUNCTION());
extern soa::debug::Log &LOG_VERBOSE(const char *path = __builtin_FILE(), const uint32_t line = __builtin_LINE(), const char *func = __builtin_FUNCTION());

#endif // ___DEBUG_LOG_HPP___
