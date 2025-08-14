#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Api_Zone5Hvac.hpp>
#include <Debug_DataType2String.hpp>

static int cmd_Ecu_Methods(std::list<std::string> argList)
{
	std::stringstream ss;
	if (argList.size() < 2)
	{
		ss << "Usage: ecu cmd [power|ac|auto|sync|driver|passenger|blowforce|blowdir|airsrc|fwddefrost|bckdefrost] [args]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string cmd = std::move(argList.front());
	argList.pop_front();
	std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });
	std::string arg = std::move(argList.front());
	argList.pop_front();
	std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

	if (cmd == "power")
	{
		bool onOff = (arg == "on");
		ss << "Zone5_HVAC_SetHvacPower(" << getStringEnum_bool(onOff) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetHvacPower(onOff);
	}
	else if (cmd == "ac") {
		bool onOff = (arg == "on");
		ss << "Zone5_HVAC_SetAcOnOff(" << getStringEnum_bool(onOff) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetAcOnOff(onOff);
	}
	else if (cmd == "auto") {
		bool onOff = (arg == "on");
		ss << "Zone5_HVAC_SetAutoTemp(" << getStringEnum_bool(onOff) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetAutoTemp(onOff);
	}
	else if (cmd == "sync") {
		bool onOff = (arg == "on");
		ss << "Zone5_HVAC_SetSync(" << getStringEnum_bool(onOff) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetSync(onOff);
	}
	else if (cmd == "driver") {
		int temp;
		std::stringstream ss_num(arg);
		ss_num >> temp;
		if (ss_num.fail())
		{
			ss << "Usage: ecu " << arg << " [int value]";
			CmdMgr::GetInstance()->Println(ss.str());
			return 0;
		}

		ss << "Zone5_HVAC_SetDriverTemp(" << temp << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetDriverTemp(temp);
	}
	else if (cmd == "passenger") {
		int temp;
		std::stringstream ss_num(arg);
		ss_num >> temp;
		if (ss_num.fail())
		{
			ss << "Usage: ecu " << arg << " [int value]";
			CmdMgr::GetInstance()->Println(ss.str());
			return 0;
		}

		ss << "Zone5_HVAC_SetPassengerTemp(" << temp << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetPassengerTemp(temp);
	}
	else if (cmd == "blowforce") {
		int force;
		std::stringstream ss_num(arg);
		ss_num >> force;
		if (ss_num.fail())
		{
			ss << "Usage: ecu " << arg << " [int value]";
			CmdMgr::GetInstance()->Println(ss.str());
			return 0;
		}

		ss << "Zone5_HVAC_SetBlowingForce(" << force << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetBlowingForce(force);
	}
	else if (cmd == "blowdir") {
		HvacBlowDir_e dir;

		if (arg == "chest")
		{
			dir = eHvacBlowDir_Chest;
		}
		else if (arg == "both")
		{
			dir = eHvacBlowDir_Both;
		}
		else if (arg == "foot")
		{
			dir = eHvacBlowDir_Foot;
		}
		else if (arg == "ws")
		{
			dir = eHvacBlowDir_FootWithWS;
		}
		else
		{
			ss << "Usage: ecu " << arg << " [chest|both|foot|ws]";
			CmdMgr::GetInstance()->Println(ss.str());
			return 0;
		}

		ss << "Zone5_HVAC_SetBlowingDirection(" << getStringEnum_HvacBlowDir_e(dir) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetBlowingDirection(dir);
	}
	else if (cmd == "airsrc") {
		HvacAirSource_e src;

		if (arg == "outer")
		{
			src = eHvacAirSource_Outer;
		}
		else if (arg == "inner")
		{
			src = eHvacAirSource_Inner;
		}
		else
		{
			ss << "Usage: ecu " << arg << " [outer|inner]";
			CmdMgr::GetInstance()->Println(ss.str());
			return 0;
		}

		ss << "Zone5_HVAC_SetBlowingDirection(" << getStringEnum_HvacAirSource_e(src) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetAirSource(src);
	}
	else if (cmd == "fwddefrost") {
		bool onOff = (arg == "on");
		ss << "Zone5_HVAC_SetForwardDefrostOn(" << getStringEnum_bool(onOff) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetForwardDefrostOn(onOff);
	}
	else if (cmd == "bckdefrost") {
		bool onOff = (arg == "on");
		ss << "Zone5_HVAC_SetBackwardDefrostOn(" << getStringEnum_bool(onOff) << ")";
		CmdMgr::GetInstance()->Println(ss.str());
		Zone5_HVAC_SetBackwardDefrostOn(onOff);
	}
	else
	{
		ss << "Usage: ecu cmd [power|ac|auto|sync|driver|passenger|blowforce|blowdir|airsrc|fwddefrost|bckdefrost] [args]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	return 0;
}


static int cmd_Ecu(void *item, std::list<std::string> argList)
{
	std::stringstream ss;
	if (argList.size() <= 0)
	{
		return -1;
	}

	std::string cmd = std::move(argList.front());
	argList.pop_front();
	std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

	if (cmd == "cmd")
	{
		return cmd_Ecu_Methods(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [ecu]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

void RegisterEcuApi()
{
	CmdMgr::GetInstance()->InsertCommand("ecu",
			"ecu [get] (args...)",
			"debug for Context",
			"debug for Context",
			cmd_Ecu, nullptr);
}