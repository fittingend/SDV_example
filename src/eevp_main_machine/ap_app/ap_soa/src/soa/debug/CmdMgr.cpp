#include <Common.h>
#include <iostream>
#include <thread>
#include <list>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <MsgSender.hpp>
#include <CmdMgr.hpp>
#include <Log.hpp>

CmdMgr::CmdMgr(MsgSender *msgSender)
{
    this->mSender = msgSender;
    this->mCmdList.clear();
}

CmdMgr::~CmdMgr()
{

}

void CmdMgr::InsertCommand(std::string cmd, std::string format, std::string shortDes, std::string longDes, std::function<int(std::list<std::string>)> func)
{
    CmdItem_t *found = nullptr;

    for (CmdItem_t *item : this->mCmdList)
    {
        if (item->mCmd == cmd)
        {
            found = item;
            break;
        }
    }

    if (found == nullptr)
    {
        found = new CmdItem_t();
        found->mCmd = cmd;
        this->mCmdList.push_back(found);
    }

    found->mFormat = format;
    found->mShortDesc = shortDes;
    found->mLongDesc = longDes;
    found->mFunc = func;
}

void CmdMgr::ExecuteCommand(char cmdline[])
{
    this->parseInputString(cmdline);

    if (this->mCmd == "help")
    {
        this->DisplayHelp();
    }
    else
    {
        for (CmdItem_t *item : this->mCmdList)
        {
            if (item->mCmd == this->mCmd)
            {
                std::ostringstream ss;
                int ret = item->mFunc(this->mArguments);
                if (ret == -1)
                {
                    ss << "Wrong usage in cmd [" << item->mCmd << "] ... \r\n";
                    ss << "Usage: " << item->mFormat << "\r\n";
                    this->mSender->SendMessage(ss.str());
                }
                return;
            }
        }

        this->DisplayNotFoundMsg();
    }
}

std::string CmdMgr::Trim(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

int CmdMgr::parseInputString(char cmdLine[])
{
    std::string input_str(cmdLine);
    std::string trimmed = this->Trim(input_str);
    std::istringstream iss(trimmed);
    std::string word;
    std::list<std::string> wordList;

    wordList.clear();
    while (iss >> word)
    {
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        wordList.push_back(word);
    }

    if (wordList.size() == 0)
    {
        LOG_ERROR() << "[CmdMgr::parseInputString] wordList size = 0 : no keyword";
        return -1;
    }

    this->mCmd = wordList.front();
    wordList.pop_front();
    this->mArguments = std::move(wordList);

    return 0;
}

void CmdMgr::Print(std::string log)
{
    this->mSender->SendMessage(log);
}

void CmdMgr::DisplayHelp(void)
{
    std::ostringstream ss;

    ss << "Command Lists:\r\n";
    ss << "---------------------------------------------------\r\n";
    ss << "  help : display all the registered commands.\r\n";
    for (CmdItem_t *item : this->mCmdList)
    {
        ss << "  " << item->mCmd << " : " << item->mShortDesc << "\r\n";
    }

    this->mSender->SendMessage(ss.str());
}

void CmdMgr::DisplayNotFoundMsg(void)
{
    std::ostringstream ss;

    ss << "Not registered command: [" << this->mCmd << "]\r\n";
    this->mSender->SendMessage(ss.str());
}