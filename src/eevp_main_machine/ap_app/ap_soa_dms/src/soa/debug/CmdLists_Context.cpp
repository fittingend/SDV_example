#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <string>
#include <string.h>
#include <DmsManager.hpp>
#include <Api_SoaDms.hpp>
#include <Api_Zone5Dms.hpp>
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
	DeviceNormal_e isNormal = VehicleContext::GetInstance()->mDeviceNormal;

	ss << "DeviceNormal changes [" << getStringEnum_DeviceNormal_e(isNormal) << "] >> [";

	switch (isNormal)
	{
	case eDeviceNormal_Ok:
		isNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		isNormal = eDeviceNormal_Ok;
		break;
	}

	ss << getStringEnum_DeviceNormal_e(isNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	DmsManager::GetInstance()->DeviceNormalChanged(isNormal);
	return 0;
};

static int cmd_Context_change_dms (std::list<std::string> &argList)
{
	std::stringstream ss;
	bool powerOn = VehicleContext::GetInstance()->mPowerOn;
	DmsDriverInside_e inside = VehicleContext::GetInstance()->mDriverInside;

	ss << "mPowerOn changes [" << getStringEnum_bool(powerOn) << "] >> [";
	powerOn = (powerOn) ? false : true;
	ss << getStringEnum_bool(powerOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mDriverInside changes [" << getStringEnum_DmsDriverInside_e(inside) << "] >> [";
	switch (inside)
	{
	case eDmsDriverInside_Unknown:
		inside = eDmsDriverInside_DriverIn;
		break;
	case eDmsDriverInside_DriverIn:
		inside = eDmsDriverInside_None;
		break;
	case eDmsDriverInside_None:
	default:
		inside = eDmsDriverInside_Unknown;
		break;
	}
	ss << getStringEnum_DmsDriverInside_e(inside) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	DmsManager::GetInstance()->DmsStatusChanged(powerOn, inside);
	return 0;
};

static int cmd_Context_change_driver (std::list<std::string> &argList)
{
	std::stringstream ss;
	DmsGazingDir_e dir = VehicleContext::GetInstance()->mGazingDir;
	DmsFatigue_e fatigue = VehicleContext::GetInstance()->mFatigue;
	DmsSmoking_e smoking = VehicleContext::GetInstance()->mIsSmoking;
	DmsOnPhone_e phone = VehicleContext::GetInstance()->mIsOnPhone;

	ss << "mGazingDir changes [" << getStringEnum_DmsGazingDir_e(dir) << "] >> [";
	switch (dir)
	{
	case eDmsGazingDir_Unknown:
		dir = eDmsGazingDir_Front;
		break;
	case eDmsGazingDir_Front:
		dir = eDmsGazingDir_FrontLeft;
		break;
	case eDmsGazingDir_FrontLeft:
		dir = eDmsGazingDir_FrontRight;
		break;
	case eDmsGazingDir_FrontRight:
		dir = eDmsGazingDir_RearMirror;
		break;
	case eDmsGazingDir_RearMirror:
		dir = eDmsGazingDir_LeftMirror;
		break;
	case eDmsGazingDir_LeftMirror:
		dir = eDmsGazingDir_RightMirror;
		break;
	case eDmsGazingDir_RightMirror:
		dir = eDmsGazingDir_DashBoard;
		break;
	case eDmsGazingDir_DashBoard:
		dir = eDmsGazingDir_Media;
		break;
	case eDmsGazingDir_Media:
		dir = eDmsGazingDir_Other;
		break;
	case eDmsGazingDir_Other:
	default:
		dir = eDmsGazingDir_Unknown;
		break;
	}
	ss << getStringEnum_DmsGazingDir_e(dir) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mFatigue changes [" << getStringEnum_DmsFatigue_e(fatigue) << "] >> [";
	switch (fatigue)
	{
	case eDmsFatigue_Unknown:
		fatigue = eDmsFatigue_Ok;
		break;
	case eDmsFatigue_Ok:
		fatigue = eDmsFatigue_Drowsy;
		break;
	case eDmsFatigue_Drowsy:
		fatigue = eDmsFatigue_Sleep;
		break;
	case eDmsFatigue_Sleep:
	default:
		fatigue = eDmsFatigue_Unknown;
		break;
	}
	ss << getStringEnum_DmsFatigue_e(fatigue) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mIsOnPhone changes [" << getStringEnum_DmsOnPhone_e(phone) << "] >> [";
	switch (phone)
	{
	case eDmsOnPhone_Unknown:
		phone = eDmsOnPhone_OnPhone;
		break;
	case eDmsOnPhone_OnPhone:
		phone = eDmsOnPhone_NotOnPhone;
		break;
	case eDmsOnPhone_NotOnPhone:
	default:
		phone = eDmsOnPhone_Unknown;
		break;
	}
	ss << getStringEnum_DmsOnPhone_e(phone) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mIsSmoking changes [" << getStringEnum_DmsSmoking_e(smoking) << "] >> [";
	switch (smoking)
	{
	case eDmsSmoking_Unknown:
		smoking = eDmsSmoking_Smoking;
		break;
	case eDmsSmoking_Smoking:
		smoking = eDmsSmoking_NoSmoking;
		break;
	case eDmsSmoking_NoSmoking:
	default:
		smoking = eDmsSmoking_Unknown;
		break;
	}
	ss << getStringEnum_DmsSmoking_e(smoking) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	DmsManager::GetInstance()->DriverStatusChanged(dir, fatigue, smoking, phone);
	return 0;
};

static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
	ss << "Usage: context change [isnormal|dms|driver]";
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
	else if (arg == "dms")
	{
		return cmd_Context_change_dms(argList);
	}
	else if (arg == "driver")
	{
		return cmd_Context_change_driver(argList);
	}

	ss << "Usage: context change [isnormal|dms|driver]";
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_notify (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context notify [nofocus|sleep|nowatch]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg = std::move(argList.front());
	argList.pop_front();
	std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

	if (arg == "nofocus")
	{
		DmsManager::GetInstance()->NotifyOnDriverNotFocusing();
		return 0;
	}
	else if (arg == "sleep")
	{
		DmsManager::GetInstance()->NotifyOnDriverNotWaken();
		return 0;
	}
	else if (arg == "nowatch")
	{
		DmsManager::GetInstance()->NotifyOnDriverNotWatchingFront();
		return 0;
	}

	ss << "Usage: context notify [nofocus|sleep|nowatch]";
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
	else if (cmd == "notify")
	{
		return cmd_Context_notify(argList);
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
			"context [status|change|notify] (args...)",
			"debug for Context",
			"debug for Context",
			cmd_Context, nullptr);
}