#include <Common.h>
#include <SoaDataType.h>
#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <random>
#include <string>
#include <string.h>
#include <SmartFilmManager.hpp>
#include <Debug_DataType2String.hpp>
#include <VehicleContext.hpp>

// status
static int cmd_Context_Status (std::list<std::string> &argList)
{
	VehicleContext *context = VehicleContext::GetInstance();
	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
	return 0;
};

// change
static int cmd_Context_change_isnormal (std::list<std::string> &argList)
{
	std::stringstream ss;
	DeviceNormal_e deviceNormal = VehicleContext::GetInstance()->mDeviceNormal;

	ss << "DeviceNormal changes [" << getStringEnum_DeviceNormal_e(deviceNormal) << "] >> [";

	switch (deviceNormal)
	{
	case eDeviceNormal_Ok:
		deviceNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		deviceNormal = eDeviceNormal_Ok;
		break;
	}

	ss << getStringEnum_DeviceNormal_e(deviceNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SmartFilmManager::GetInstance()->UpdateDeviceNormal(deviceNormal);
	return 0;
};

static int cmd_Context_change_auto(std::list<std::string> &argList)
{
	std::stringstream ss;
	bool autoMode = VehicleContext::GetInstance()->mAuto;

	ss << "AutoMode changes [" << getStringEnum_bool(autoMode) << "] >> [";
	autoMode = !autoMode;
	ss << getStringEnum_bool(autoMode) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SmartFilmManager::GetInstance()->UpdateAutoMode(autoMode);
	return 0;
};

static int cmd_Context_change_opacity(std::list<std::string> &argList)
{
	std::stringstream ss;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 100);
	SmtfilmOpacities_t opacities = VehicleContext::GetInstance()->mOpacities;

	for (int i = 0; i < (int)eSmtfilePos_MAX; i++)
	{
		ss << "Opacity[" << i << "] = " << opacities.mOpacities[i];
		opacities.mOpacities[i] = dist(gen);
		ss << " -> " << opacities.mOpacities[i];
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}

	SmartFilmManager::GetInstance()->UpdateSmtfilmOpacities(opacities);
	return 0;
};

static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context change [isnormal|auto|opacity]";
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
	else if (arg == "auto")
	{
		return cmd_Context_change_auto(argList);
	}
	else if (arg == "opacity")
	{
		return cmd_Context_change_opacity(argList);
	}

	ss << "Usage: context change [isnormal|auto|opacity]";
	CmdMgr::GetInstance()->Println(ss.str());
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
			"context [status|change] (args...)",
			"debug for Context",
			"debug for Context",
			cmd_Context, nullptr);
}