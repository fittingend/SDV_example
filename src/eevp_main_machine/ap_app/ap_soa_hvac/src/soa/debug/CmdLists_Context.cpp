#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <random>
#include <string>
#include <string.h>
#include <HvacManager.hpp>
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

	HvacManager::GetInstance()->UpdateDeviceNormal(deviceNormal);
	return 0;
};

static int cmd_Context_change_setting(std::list<std::string> &argList)
{
	std::stringstream ss;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(3, 10);
	HvacStatus_t status = VehicleContext::GetInstance()->mSetting;

	ss << "Setting Changes:\n";
	ss << "mPowerOn changes [" << getStringEnum_bool(status.mPowerOn) << "] >> [";
	status.mPowerOn = (status.mPowerOn) ? false : true;
	ss << getStringEnum_bool(status.mPowerOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mAcOnOff changes [" << getStringEnum_bool(status.mAcOnOff) << "] >> [";
	status.mAcOnOff = (status.mAcOnOff) ? false : true;
	ss << getStringEnum_bool(status.mAcOnOff) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mAutoMode changes [" << getStringEnum_bool(status.mAutoMode) << "] >> [";
	status.mAutoMode = (status.mAutoMode) ? false : true;
	ss << getStringEnum_bool(status.mAutoMode) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mSyncMode changes [" << getStringEnum_bool(status.mSyncMode) << "] >> [";
	status.mSyncMode = (status.mSyncMode) ? false : true;
	ss << getStringEnum_bool(status.mSyncMode) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mDriverTemp changes [" << status.mDriverTemp << "] >> [";
	if ((status.mDriverTemp < 10) || (status.mDriverTemp > 35))
	{
		status.mDriverTemp = 10;
	}
	else
	{
		status.mDriverTemp += dist(gen);
	}
	ss << status.mDriverTemp << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mPassengerTemp changes [" << status.mPassengerTemp << "] >> [";
	if ((status.mPassengerTemp < 10) || (status.mPassengerTemp > 35))
	{
		status.mPassengerTemp = 10;
	}
	else
	{
		status.mPassengerTemp += dist(gen);
	}
	ss << status.mPassengerTemp << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBlowingForce changes [" << status.mBlowingForce << "] >> [";
	status.mBlowingForce += 10;
	if (status.mBlowingForce > 100)
	{
		status.mBlowingForce = 0;
	}
	ss << status.mBlowingForce << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBlowDir changes [" << getStringEnum_HvacBlowDir_e(status.mBlowDir) << "] >> [";
	switch (status.mBlowDir)
	{
	case eHvacBlowDir_Chest:
		status.mBlowDir = eHvacBlowDir_Both;
		break;
	case eHvacBlowDir_Both:
		status.mBlowDir = eHvacBlowDir_Foot;
		break;
	case eHvacBlowDir_Foot:
		status.mBlowDir = eHvacBlowDir_FootWithWS;
		break;
	case eHvacBlowDir_FootWithWS:
	default:
		status.mBlowDir = eHvacBlowDir_Chest;
		break;
	}
	ss << getStringEnum_HvacBlowDir_e(status.mBlowDir) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBlowDir changes [" << getStringEnum_HvacAirSource_e(status.mAirSrc) << "] >> [";
	switch (status.mAirSrc)
	{
	case eHvacAirSource_Outer:
		status.mAirSrc = eHvacAirSource_Inner;
		break;
	case eHvacAirSource_Inner:
	default:
		status.mAirSrc = eHvacAirSource_Outer;
		break;
	}
	ss << getStringEnum_HvacAirSource_e(status.mAirSrc) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mForwardFrostOn changes [" << getStringEnum_bool(status.mForwardFrostOn) << "] >> [";
	status.mForwardFrostOn = (status.mForwardFrostOn) ? false : true;
	ss << getStringEnum_bool(status.mForwardFrostOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBackwardFrostOn changes [" << getStringEnum_bool(status.mBackwardFrostOn) << "] >> [";
	status.mBackwardFrostOn = (status.mBackwardFrostOn) ? false : true;
	ss << getStringEnum_bool(status.mBackwardFrostOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	HvacManager::GetInstance()->UpdateHvacSetting(status);
	return 0;
};

static int cmd_Context_change_status(std::list<std::string> &argList)
{
	std::stringstream ss;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(3, 10);
	HvacStatus_t status = VehicleContext::GetInstance()->mStatus;

	ss << "Status Changes:\n";
	ss << "mPowerOn changes [" << getStringEnum_bool(status.mPowerOn) << "] >> [";
	status.mPowerOn = (status.mPowerOn) ? false : true;
	ss << getStringEnum_bool(status.mPowerOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mAcOnOff changes [" << getStringEnum_bool(status.mAcOnOff) << "] >> [";
	status.mAcOnOff = (status.mAcOnOff) ? false : true;
	ss << getStringEnum_bool(status.mAcOnOff) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mAutoMode changes [" << getStringEnum_bool(status.mAutoMode) << "] >> [";
	status.mAutoMode = (status.mAutoMode) ? false : true;
	ss << getStringEnum_bool(status.mAutoMode) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mSyncMode changes [" << getStringEnum_bool(status.mSyncMode) << "] >> [";
	status.mSyncMode = (status.mSyncMode) ? false : true;
	ss << getStringEnum_bool(status.mSyncMode) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mDriverTemp changes [" << status.mDriverTemp << "] >> [";
	if ((status.mDriverTemp < 10) || (status.mDriverTemp > 35))
	{
		status.mDriverTemp = 10;
	}
	else
	{
		status.mDriverTemp += dist(gen);
	}
	ss << status.mDriverTemp << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mPassengerTemp changes [" << status.mPassengerTemp << "] >> [";
	if ((status.mPassengerTemp < 10) || (status.mPassengerTemp > 35))
	{
		status.mPassengerTemp = 10;
	}
	else
	{
		status.mPassengerTemp += dist(gen);
	}
	ss << status.mPassengerTemp << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBlowingForce changes [" << status.mBlowingForce << "] >> [";
	status.mBlowingForce += 10;
	if (status.mBlowingForce > 100)
	{
		status.mBlowingForce = 0;
	}
	ss << status.mBlowingForce << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBlowDir changes [" << getStringEnum_HvacBlowDir_e(status.mBlowDir) << "] >> [";
	switch (status.mBlowDir)
	{
	case eHvacBlowDir_Chest:
		status.mBlowDir = eHvacBlowDir_Both;
		break;
	case eHvacBlowDir_Both:
		status.mBlowDir = eHvacBlowDir_Foot;
		break;
	case eHvacBlowDir_Foot:
		status.mBlowDir = eHvacBlowDir_FootWithWS;
		break;
	case eHvacBlowDir_FootWithWS:
	default:
		status.mBlowDir = eHvacBlowDir_Chest;
		break;
	}
	ss << getStringEnum_HvacBlowDir_e(status.mBlowDir) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBlowDir changes [" << getStringEnum_HvacAirSource_e(status.mAirSrc) << "] >> [";
	switch (status.mAirSrc)
	{
	case eHvacAirSource_Outer:
		status.mAirSrc = eHvacAirSource_Inner;
		break;
	case eHvacAirSource_Inner:
	default:
		status.mAirSrc = eHvacAirSource_Outer;
		break;
	}
	ss << getStringEnum_HvacAirSource_e(status.mAirSrc) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mForwardFrostOn changes [" << getStringEnum_bool(status.mForwardFrostOn) << "] >> [";
	status.mForwardFrostOn = (status.mForwardFrostOn) ? false : true;
	ss << getStringEnum_bool(status.mForwardFrostOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mBackwardFrostOn changes [" << getStringEnum_bool(status.mBackwardFrostOn) << "] >> [";
	status.mBackwardFrostOn = (status.mBackwardFrostOn) ? false : true;
	ss << getStringEnum_bool(status.mBackwardFrostOn) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	HvacManager::GetInstance()->UpdateHvacStatus(status);
	return 0;
};

static int cmd_Context_change_temps(std::list<std::string> &argList)
{
	std::stringstream ss;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(-7, 7);
	HvacTemps_t temps = VehicleContext::GetInstance()->mTemps;

	ss << "Temps Changes:\n";
	ss << "mInside changes [" << temps.mInside << "] >> [";
	if ((temps.mInside <= 0) || (temps.mInside > 40))
	{
		temps.mInside = 25;
	}
	else
	{
		temps.mInside += dist(gen);
	}
	ss << temps.mInside << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mOutside changes [" << temps.mOutside << "] >> [";
	if ((temps.mOutside <= 0) || (temps.mOutside > 40))
	{
		temps.mOutside = 25;
	}
	else
	{
		temps.mOutside += dist(gen);
	}
	ss << temps.mOutside << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mDuct changes [" << temps.mDuct << "] >> [";
	if ((temps.mDuct <= 0) || (temps.mDuct > 40))
	{
		temps.mDuct = 25;
	}
	else
	{
		temps.mDuct += dist(gen);
	}
	ss << temps.mDuct << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mEvaporator changes [" << temps.mEvaporator << "] >> [";
	if ((temps.mEvaporator <= 0) || (temps.mEvaporator > 40))
	{
		temps.mEvaporator = 25;
	}
	else
	{
		temps.mEvaporator += dist(gen);
	}
	ss << temps.mEvaporator << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mDriverVentDuct changes [" << temps.mDriverVentDuct << "] >> [";
	if ((temps.mDriverVentDuct <= 0) || (temps.mDriverVentDuct > 40))
	{
		temps.mDriverVentDuct = 25;
	}
	else
	{
		temps.mDriverVentDuct += dist(gen);
	}
	ss << temps.mDriverVentDuct << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mDriverFloorDuct changes [" << temps.mDriverFloorDuct << "] >> [";
	if ((temps.mDriverFloorDuct <= 0) || (temps.mDriverFloorDuct > 40))
	{
		temps.mDriverFloorDuct = 25;
	}
	else
	{
		temps.mDriverFloorDuct += dist(gen);
	}
	ss << temps.mDriverFloorDuct << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mPassengerVentDuct changes [" << temps.mPassengerVentDuct << "] >> [";
	if ((temps.mPassengerVentDuct <= 0) || (temps.mPassengerVentDuct > 40))
	{
		temps.mPassengerVentDuct = 25;
	}
	else
	{
		temps.mPassengerVentDuct += dist(gen);
	}
	ss << temps.mPassengerVentDuct << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "mPassengerFloorDuct changes [" << temps.mPassengerFloorDuct << "] >> [";
	if ((temps.mPassengerFloorDuct <= 0) || (temps.mPassengerFloorDuct > 40))
	{
		temps.mPassengerFloorDuct = 25;
	}
	else
	{
		temps.mPassengerFloorDuct += dist(gen);
	}
	ss << temps.mPassengerFloorDuct << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	HvacManager::GetInstance()->UpdateHvacTemps(temps);
	return 0;
};


static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context change [isnormal|setting|status|temps]";
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
	else if (arg == "setting")
	{
		return cmd_Context_change_setting(argList);
	}
	else if (arg == "status")
	{
		return cmd_Context_change_status(argList);
	}
	else if (arg == "temps")
	{
		return cmd_Context_change_temps(argList);
	}

	ss << "Usage: context change [isnormal|setting|status|temps]";
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