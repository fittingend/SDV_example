#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Zone2RoaProxyImpl.hpp>

static int cmd_roa_status (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone2::roa::control::Zone2RoaProxyImpl::GetInstance();
	zone2::roa::control::EcmRoaSnsrStatus snsrStatus;

	ss << "getterSnsrStatus (+)";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	bool ret = inst->getterSnsrStatus(snsrStatus);

	ss << "getterSnsrStatus (-) ret=" << (ret == true) ? "TRUE" : "FALSE";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "detInfo = " << (int)snsrStatus.detInfo;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "snsrStat = " << (int)snsrStatus.snsrStat;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "snsrErrState = " << (int)snsrStatus.snsrErrState;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "isNormal = " << (int)snsrStatus.isNormal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	return 0;
}

static int cmd_roa_control (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone2::roa::control::Zone2RoaProxyImpl::GetInstance();
	if (!inst->isServiceFound())
	{
		ss << "Zone2Roa Service not found" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	if (argList.size() <= 0)
	{
		ss << "Usage: ecu control [control_op]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	std::string arg = std::move(argList.front());
	argList.pop_front();

	int num;
	bool valid;
	std::stringstream ss2(arg);
	ss2 >> num;
	if (ss2.fail())
	{
		ss << "Usage: ecu control [control_op]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	inst->controlRoa((std::uint8_t)num);

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
		return cmd_roa_control(argList);
	}
	else if (cmd == "status")
	{
		return cmd_roa_status(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [ecu]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

class RoaListener : public zone2::roa::control::IZone2RoaListener
{
public:
	std::uint8_t detInfo;
	std::uint8_t snsrStat;
	std::uint8_t snsrErrState;
	std::uint8_t isNormal;

	void notifySnsrStatus(const zone2::roa::control::EcmRoaSnsrStatus &fieldValue) override
	{
		std::stringstream ss;
		ss << "notifySnsrStatus (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "SnsrStatus detInfo = " << (int)fieldValue.detInfo;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "SnsrStatus snsrStat = " << (int)fieldValue.snsrStat;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "SnsrStatus snsrErrState = " << (int)fieldValue.snsrErrState;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "SnsrStatus isNormal = " << (int)fieldValue.isNormal;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "notifySnsrStatus (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

	}

	void getSnsrStatus() override
	{

	}
};

void RegisterEcuApi()
{
	CmdMgr::GetInstance()->InsertCommand("ecu",
			"ecu [control|status] (args...)",
			"debug for ECU ROA APIs",
			"debug for ECU ROA APIs",
			cmd_Ecu, nullptr);

	auto *inst = zone2::roa::control::Zone2RoaProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<RoaListener>());
}