#ifndef __CMD_MGR_HPP__
#define __CMD_MGR_HPP__

#include <Common.h>
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
    std::function<int(void *, std::list<std::string>)> mFunc;
    void *mItem;
} CmdItem_t;

typedef struct
{
    std::string mGroup;
    std::string mCmd;
    std::string mFormat;
    std::string mShortDesc;
    std::string mLongDesc;
    //RemoteSender *mItem;
} GroupItem_t;

class CmdMgr
{
private:
    static CmdMgr *mInst;
    MsgSender *mSender;

    std::string mCurrGroupStr;
    bool mIsBypassMode;

    std::string mCmd;
    std::list<std::string> mArguments;
    std::list<CmdItem_t *> mCmdList;
    std::list<GroupItem_t *> mGroupList;

    CmdMgr();
    ~CmdMgr();
    std::string trim(const std::string &str);
    int parseInputString(char cmdLine[]);
    void displayHelp();
    void displayNotFoundMsg();
    void executeExit();
    void setGroup(std::list<std::string> group_arg);
    CmdItem_t *findCommandItem(std::string cmd);
    GroupItem_t *findGroupItem(std::string group);

public:
    static CmdMgr *GetInstance();
    static void DestroyInstance();
    void RegisterMsgSender(MsgSender *msgSender);
    void InsertCommand(std::string cmd, std::string format, std::string shortDes, std::string longDes, std::function<int(void *, std::list<std::string>)> func, void *item);
    //void InsertGroup(std::string group, std::string format, std::string shortDes, std::string longDes, RemoteSender *item);
    void ExecuteCommand(char cmdline[]);
    void Print(std::string log);
    void Println(std::string log);
    bool IsCommandExist(std::string cmd);
    std::string GetPrompt(void);
};

#endif