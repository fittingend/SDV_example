#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <string>
#include <string.h>
#include <Api_SoaRcurtain.hpp>
#include <Api_Zone3Rcurtain.hpp>
#include <Debug_DataType2String.hpp>
#include <VehicleContext.hpp>

static int cmd_Context_change_isnormal (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "DeviceNormal changes [" << getStringEnum_DeviceNormal_e(context->mIsNormal) << "] >> [";

	switch (context->mIsNormal)
	{
	case eDeviceNormal_Ok:
		context->mIsNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mIsNormal = eDeviceNormal_Ok;
		break;
	}

	ss << getStringEnum_DeviceNormal_e(context->mIsNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_REARCURTAIN_UpdateStatus();
	return 0;
};

static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context change [isnormal]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg = std::move(argList.front());
	argList.pop_front();
	std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

	if (arg == "isnormal")
	{
		return cmd_Context_change_isnormal(argList);
	}

	ss << "Usage: context change [isnormal]";
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_reset (std::list<std::string> &argList)
{
	std::stringstream ss;

	Zone3_RearCurtain_UpdateContext();
	ss << "Context updated to the ECU zone1." ;
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_Status (std::list<std::string> &argList)
{
	VehicleContext *context = VehicleContext::GetInstance();
	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
	return 0;
};

static int cmd_Context(void *item, std::list<std::string> argList)
{
	std::stringstream ss;
	if (argList.size() <= 0)
	{
		return -1;
	}

	std::string cmd = std::move(argList.front());
	argList.pop_front();
	std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

	if (cmd == "status")
	{
		return cmd_Context_Status(argList);
	}
	else if (cmd == "change")
	{
		return cmd_Context_change(argList);
	}
	else if (cmd == "reset")
	{
		return cmd_Context_reset(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [context]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

void RegisterCmdContext()
{
	CmdMgr::GetInstance()->InsertCommand("context",
			"context [status|change|reset] (args...)",
			"debug for Context",
			"debug for Context",
			cmd_Context, nullptr);
}