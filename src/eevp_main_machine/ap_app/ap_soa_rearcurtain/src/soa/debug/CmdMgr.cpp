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
#include <TelnetServer.hpp>
#include <Log.hpp>

#define STR_GROUP_ROOT  "ROOT"

CmdMgr *CmdMgr::mInst = nullptr;

CmdMgr *CmdMgr::GetInstance()
{
	if (CmdMgr::mInst == nullptr)
	{
		CmdMgr::mInst = new CmdMgr();
	}

	return CmdMgr::mInst;
}

void CmdMgr::DestroyInstance()
{
	if (CmdMgr::mInst != nullptr)
	{
		delete CmdMgr::mInst;
		CmdMgr::mInst = nullptr;
	}
}

CmdMgr::CmdMgr()
{
	this->mSender = nullptr;
	this->mCmdList.clear();
	this->mGroupList.clear();
	this->mCurrGroupStr = STR_GROUP_ROOT;
	this->mIsBypassMode = false;
}

CmdMgr::~CmdMgr()
{

}

void CmdMgr::RegisterMsgSender(MsgSender *msgSender)
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);
	this->mSender = msgSender;
}

void CmdMgr::InsertCommand(std::string cmd, std::string format, std::string shortDes, std::string longDes, std::function<int(void *, std::list<std::string>)> func, void *item)
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);
	CmdItem_t *found = this->findCommandItem(cmd);
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
	found->mItem = item;
}


void CmdMgr::ExecuteCommand(char cmdline[])
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);

	this->parseInputString(cmdline);

	if (this->mCmd == "help")
	{
		this->displayHelp();
	}
	else if (this->mCmd == "exit")
	{
		this->executeExit();
	}
	else if (this->mIsBypassMode)
	{

	}
	else
	{
		for (CmdItem_t *item : this->mCmdList)
		{
			if (item->mCmd == this->mCmd)
			{
				std::ostringstream ss;
				int ret = item->mFunc(item->mItem, this->mArguments);
				if (ret == -1)
				{
					ss << "Wrong usage in cmd [" << item->mCmd << "] ... \r\n";
					ss << "Usage: " << item->mFormat << "\r\n";
					if (this->mSender)
					{
						this->mSender->SendMessage(ss.str());
					}
				}
				return;
			}
		}

		this->displayNotFoundMsg();
	}
}

std::string CmdMgr::trim(const std::string &str)
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
	std::string trimmed = this->trim(input_str);
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
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);
	if (this->mSender)
	{
		this->mSender->SendMessage(log);
	}
}

void CmdMgr::Println(std::string log)
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);
	std::stringstream ss;
	ss << log << "\r\n";
	if (this->mSender)
	{
		this->mSender->SendMessage(ss.str());
	}
}

void CmdMgr::displayHelp(void)
{
	std::ostringstream ss;

	ss << "Command Lists:\r\n";
	ss << "---------------------------------------------------\r\n";
	ss << "  help : display all the registered commands.\r\n";
	if (this->mIsBypassMode)
	{

	}
	else
	{
	for (CmdItem_t *item : this->mCmdList)
	{
		ss << "  " << item->mCmd << " : " << item->mShortDesc << "\r\n";
	}
	}

	ss << "  exit : exit the group or disconnect command.\r\n";
	ss << "\r\n";

	if (this->mSender)
	{
		this->mSender->SendMessage(ss.str());
	}
}

void CmdMgr::displayNotFoundMsg(void)
{
	std::ostringstream ss;

	ss << "Not registered command: [" << this->mCmd << "]\r\n";
	if (this->mSender)
	{
		this->mSender->SendMessage(ss.str());
	}
}

void CmdMgr::executeExit()
{
	if (this->mIsBypassMode)
	{

	}
	else
	{
		TelnetServer::GetInstance()->Disconnect();
	}

}

void CmdMgr::setGroup(std::list<std::string> group_arg)
{

}

CmdItem_t *CmdMgr::findCommandItem(std::string cmd)
{
	std::string cmdStr = cmd;
	std::for_each(cmdStr.begin(), cmdStr.end(), [](auto &c) { c = tolower(c); });
	for (CmdItem_t *item : this->mCmdList)
	{
		std::string itemCmd = item->mCmd;
		std::for_each(itemCmd.begin(), itemCmd.end(), [](auto &c) { c = tolower(c); });

		if (cmdStr == itemCmd)
		{
			return item;
		}
	}

	return nullptr;
}

GroupItem_t *CmdMgr::findGroupItem(std::string group)
{
	std::string groupStr = group;
	std::for_each(groupStr.begin(), groupStr.end(), [](auto &c) { c = tolower(c); });
	for (GroupItem_t *item : this->mGroupList)
	{
		std::string itemGroup = item->mGroup;
		std::for_each(itemGroup.begin(), itemGroup.end(), [](auto &c) { c = tolower(c); });

		if (groupStr == itemGroup)
		{
			return item;
		}
	}

	return nullptr;
}

bool CmdMgr::IsCommandExist(std::string cmd)
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);
	return (this->findCommandItem(cmd) != nullptr);
}

std::string CmdMgr::GetPrompt(void)
{
	std::stringstream ss;
	ss << "\r\n" << this->mCurrGroupStr << " >> ";
	return ss.str();
}
