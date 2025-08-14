#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Api_SoaDms.hpp>
#include <Log.hpp>

static int cmd_SoaApi_Power (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: soa power [api_level:1~3] [on_off:0|1]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg = std::move(argList.front());
	argList.pop_front();

	int api_level;
	std::stringstream ss2(arg);
	ss2 >> api_level;
	if (ss2.fail())
	{
		ss << "Usage: soa power [api_level:1~3] [on_off:0|1] -> api_level not number" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	ClientLevel_e client_level;
	switch (api_level)
	{
	case 1: client_level = eClientLevel_SubscribedApps; break;
	case 2: client_level = eClientLevel_GuiApps; break;
	case 3: client_level = eClientLevel_PhysicalSwitch; break;
	default:
		ss << "Usage: soa power [api_level:1~3] [on_off:0|1] -> api_level is outside 1~3" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg2 = std::move(argList.front());
	argList.pop_front();

	int onoff;
	std::stringstream ss3(arg2);
	ss3 >> onoff;
	if (ss3.fail())
	{
		ss << "Usage: soa power [api_level:1~3] [on_off:0|1] -> on_off not number" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	bool on;
	switch (onoff)
	{
	case 0: on = false; break;
	case 1: on = true; break;
	default:
		ss << "Usage: soa power [api_level:1~3] [on_off:0|1] -> on_off is outside 0~1" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	SOA_DMS_SetPower(client_level, on);
	return 0;
};

static int cmd_SoaApi(void *item, std::list<std::string> argList)
{
	std::stringstream ss;
	if (argList.size() <= 0)
	{
		return -1;
	}

	std::string cmd = std::move(argList.front());
	argList.pop_front();
	std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

	if (cmd == "power")
	{
		return cmd_SoaApi_Power(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [power]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

void RegisterSoaApi()
{
	CmdMgr::GetInstance()->InsertCommand("soa",
			"soa [power] (args...)",
			"debug for SOA APIs",
			"debug for SOA APIs",
			cmd_SoaApi, nullptr);
}