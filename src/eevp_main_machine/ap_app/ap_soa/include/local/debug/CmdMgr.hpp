#ifndef __CMD_MGR_HPP__
#define __CMD_MGR_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <list>
#include <map>
#include <sstream>
#include <functional>
#include <string.h>
#include <MsgSender.hpp>

typedef struct
{
    std::string mCmd;
    std::string mFormat;
    std::string mShortDesc;
    std::string mLongDesc;
    std::function<int(std::list<std::string>)> mFunc;
} CmdItem_t;

class CmdMgr
{
private:
    MsgSender *mSender;
    std::string mCmd;
    std::list<std::string> mArguments;
    std::list<CmdItem_t *> mCmdList;

    std::string Trim(const std::string &str);
    int parseInputString(char cmdLine[]);
    void DisplayHelp();
    void DisplayNotFoundMsg();

public:
    CmdMgr(MsgSender *msgSender);
    ~CmdMgr();
    void InsertCommand(std::string cmd, std::string format, std::string shortDes, std::string longDes, std::function<int(std::list<std::string>)> func);
    void ExecuteCommand(char cmdline[]);
    void Print(std::string log);
};

#endif