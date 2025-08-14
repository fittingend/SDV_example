#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <SoaDataType.h>
#include <Zone5SmartFilmProxyImpl.hpp>
#include <Debug_DataType2String.hpp>

static int cmd_smtfilm_status (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone5::smtfilm::control::Zone5SmartFilmProxyImpl::GetInstance();
	bool ret;

	ss << "getterDeviceNormal (+)";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	DeviceNormal_e isNormal;
	ret = inst->getterDeviceNormal(isNormal);
	if (ret)
	{
		ss << "getterDeviceNormal (-) ret=TRUE, value=" << getStringEnum_DeviceNormal_e(isNormal);
	}
	else
	{
		ss << "getterDeviceNormal (-) ret=FALSE";
	}
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SmtfilmOpacities_t opacities;
	ret = inst->getterSmtfilmOpacities(opacities);
	if (ret)
	{
		ss << "getterSmtfilmOpacities (-) ret=TRUE, value=" << getString_SmtfilmOpacities_t(opacities);
	}
	else
	{
		ss << "getterSmtfilmOpacities (-) ret=FALSE";
	}
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	return 0;
}

static int cmd_smtfilm_control (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone5::smtfilm::control::Zone5SmartFilmProxyImpl::GetInstance();

	if (argList.size() <= 1)
	{
		ss << "Usage: ecu control [ws|rg|dsf|dsq|asf|asq|rear] [opacity]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg1 = std::move(argList.front());
	argList.pop_front();

	SmtfilmPos_e pos;
	if (arg1 == "ws")
	{
		pos = eSmtfilmPos_WindShield;
	}
	else if (arg1 == "rg")
	{
		pos = eSmtfilmPos_RoofGlass;
	}
	else if (arg1 == "dsf")
	{
		pos = eSmtfilmPos_DsFrontRear;
	}
	else if (arg1 == "dsq")
	{
		pos = eSmtfilmPos_DsQuarter;
	}
	else if (arg1 == "asf")
	{
		pos = eSmtfilmPos_AsFrontRear;
	}
	else if (arg1 == "asq")
	{
		pos = eSmtfilmPos_AsQuarter;
	}
	else if (arg1 == "rear")
	{
		pos = eSmtfilmPos_RearGrass;
	}
	else
	{
		ss << "Usage: ecu control [ws|rg|dsf|dsq|asf|asq|rear] [opacity(int)]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg2 = std::move(argList.front());
	argList.pop_front();

	int opacity;
	std::stringstream ss2(arg2);
	ss2 >> opacity;
	if (ss2.fail())
	{
		ss << "Usage: ecu control [ws|rg|dsf|dsq|asf|asq|rear] [opacity]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	ss << "controlFilm: " << getStringEnum_SmtfilmPos_e(pos) << " opacity=" << opacity << "\n";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	inst->SetOpacity(pos, opacity);
	return 0;
};

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

	if (cmd == "control")
	{
		return cmd_smtfilm_control(argList);
	}
	else if (cmd == "status")
	{
		return cmd_smtfilm_status(argList);
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
		"ecu [control|status] (args...)",
		"debug for ECU Wiper APIs",
		"debug for ECU Wiper Seat APIs",
		cmd_Ecu, nullptr);

	//auto *inst = zone4::wiper::control::Zone4WiperProxyImpl::GetInstance();
	//inst->setEventListener(std::make_shared<WiperListener>());
}