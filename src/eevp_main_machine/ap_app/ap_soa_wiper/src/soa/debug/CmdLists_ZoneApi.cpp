#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Zone4WiperProxyImpl.hpp>

static int cmd_wiper_status (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone4::wiper::control::Zone4WiperProxyImpl::GetInstance();
	zone4::wiper::control::EcuWiperOperationStatus status;

	ss << "getterWiperOpStatus (+)";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	bool ret = inst->getterWiperOpStatus(status);

	ss << "getterWiperOpStatus (-) ret=" << (ret == true) ? "TRUE" : "FALSE";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "curOperationMode = " << (int)status.curOperationMode;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "isNormal = " << (int)status.isNormal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	return 0;
}

static int cmd_wiper_control (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone4::wiper::control::Zone4WiperProxyImpl::GetInstance();
	if (!inst->isServiceFound())
	{
		ss << "Zone4Wiper Service not found" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	if (argList.size() <= 0)
	{
		ss << "Usage: ecu control [wiper_op]" ;
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
		ss << "Usage: ecu control [wiper_op]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	inst->controlWiper((std::uint8_t)num);

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
		return cmd_wiper_control(argList);
	}
	else if (cmd == "status")
	{
		return cmd_wiper_status(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [ecu]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

class WiperListener : public zone4::wiper::control::IZone4WiperListener
{
public:
	std::uint8_t curOperationMode;
	std::uint8_t isNormal;

	void notifyWiperOpStatus(const zone4::wiper::control::EcuWiperOperationStatus &fieldValue) override
	{
		std::stringstream ss;
		ss << "notifyWiperOpStatus (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "WiperOpStatus curOperationMode = " << (int)fieldValue.curOperationMode;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "WiperOpStatus isNormal = " << (int)fieldValue.isNormal;
		CmdMgr::GetInstance()->Println(ss.str());

		ss << "notifyWiperOpStatus (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

	}

	void getWiperOpStatus()
	{

	}
};

void RegisterEcuApi()
{
	CmdMgr::GetInstance()->InsertCommand("ecu",
			"ecu [control|status] (args...)",
			"debug for ECU Wiper APIs",
			"debug for ECU Wiper Seat APIs",
			cmd_Ecu, nullptr);

	auto *inst = zone4::wiper::control::Zone4WiperProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<WiperListener>());
}