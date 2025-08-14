#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <string>
#include <string.h>
#include <Api_SoaReserved.hpp>
#include <Api_ZoneReserved.hpp>
#include <Debug_DataType2String.hpp>

#include <VehicleContext.hpp>

static int cmd_Context_change_zone1 (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Zone1 changes [" << getStringEnum_DeviceNormal_e(context->mZone1PowerNormal) << "] >> [";
	context->mZone1Connected = true;
	switch (context->mZone1PowerNormal)
	{
	case eDeviceNormal_Ok:
		context->mZone1PowerNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mZone1PowerNormal = eDeviceNormal_Ok;
		break;
	}
	ss << getStringEnum_DeviceNormal_e(context->mZone1PowerNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_ReservedA_UpdatePowerDeviceNormal();

	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
	return 0;
}

static int cmd_Context_change_zone2 (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Zone2 changes [" << getStringEnum_DeviceNormal_e(context->mZone2PowerNormal) << "] >> [";
	context->mZone2Connected = true;
	switch (context->mZone2PowerNormal)
	{
	case eDeviceNormal_Ok:
		context->mZone2PowerNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mZone2PowerNormal = eDeviceNormal_Ok;
		break;
	}
	ss << getStringEnum_DeviceNormal_e(context->mZone2PowerNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_ReservedA_UpdatePowerDeviceNormal();

	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
	return 0;
}

static int cmd_Context_change_zone3 (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Zone3 changes [" << getStringEnum_DeviceNormal_e(context->mZone3PowerNormal) << "] >> [";
	context->mZone3Connected = true;
	switch (context->mZone3PowerNormal)
	{
	case eDeviceNormal_Ok:
		context->mZone3PowerNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mZone3PowerNormal = eDeviceNormal_Ok;
		break;
	}
	ss << getStringEnum_DeviceNormal_e(context->mZone3PowerNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_ReservedA_UpdatePowerDeviceNormal();

	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
	return 0;
}

static int cmd_Context_change_zone4 (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Zone4 changes [" << getStringEnum_DeviceNormal_e(context->mZone4PowerNormal) << "] >> [";
	context->mZone4Connected = true;
	switch (context->mZone4PowerNormal)
	{
	case eDeviceNormal_Ok:
		context->mZone4PowerNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mZone4PowerNormal = eDeviceNormal_Ok;
		break;
	}
	ss << getStringEnum_DeviceNormal_e(context->mZone4PowerNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_ReservedA_UpdatePowerDeviceNormal();

	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
	return 0;
}

static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context change [zone1|zone2|zone3|zone4]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg = std::move(argList.front());
	argList.pop_front();
	std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

	if (arg == "zone1")
	{
		return cmd_Context_change_zone1(argList);
	}
	else if (arg == "zone2")
	{
		return cmd_Context_change_zone2(argList);
	}
	else if (arg == "zone3")
	{
		return cmd_Context_change_zone3(argList);
	}
	else if (arg == "zone4")
	{
		return cmd_Context_change_zone4(argList);
	}

	ss << "Usage: context change [zone1|zone2|zone3|zone4]";
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_reset (std::list<std::string> &argList)
{
	std::stringstream ss;

	ZONE_RESERVED_UpdateContext();
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