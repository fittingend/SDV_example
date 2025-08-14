#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <string>
#include <string.h>
#include <VehicleContext.hpp>
#include <Api_SoaWiper.hpp>
#include <Api_Zone4Wiper.hpp>
#include <Debug_DataType2String.hpp>

static int cmd_Context_change_isnormal (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "DeviceNormal changes [" << getStringEnum_DeviceNormal_e(context->mDeviceNormal) << "] >> [";

	switch (context->mDeviceNormal)
	{
	case eDeviceNormal_Ok:
		context->mDeviceNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mDeviceNormal = eDeviceNormal_Ok;
		break;
	}

	ss << getStringEnum_DeviceNormal_e(context->mDeviceNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_Wiper_UpdateDeviceNormal();
	return 0;
};

static int cmd_Context_change_opmode(std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Slide Direction changes [" << getStringEnum_WiperOpMode_e(context->mWiperOpMode) << "] >> [";
	switch (context->mWiperOpMode)
	{
	case eWiperOpMode_Off:
		context->mWiperOpMode = eWiperOpMode_Low;
		break;
	case eWiperOpMode_Low:
		context->mWiperOpMode = eWiperOpMode_Int1;
		break;
	case eWiperOpMode_Int1:
		context->mWiperOpMode = eWiperOpMode_Int2;
		break;
	case eWiperOpMode_Int2:
		context->mWiperOpMode = eWiperOpMode_Int3;
		break;
	case eWiperOpMode_Int3:
		context->mWiperOpMode = eWiperOpMode_Off;
		break;
	default:
		context->mWiperOpMode = eWiperOpMode_Off;
		break;
	}
	ss << getStringEnum_WiperOpMode_e(context->mWiperOpMode) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_Wiper_UpdateWiperStatus();
	return 0;
};

static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context change [isnormal|opmode]";
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
	else if (arg == "opmode")
	{
		return cmd_Context_change_opmode(argList);
	}


	ss << "Usage: context change [isnormal|opmode]";
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_reset (std::list<std::string> &argList)
{
	std::stringstream ss;

	Zone4_Wiper_UpdateContext();
	ss << "Context updated to the ECU zone4." ;
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