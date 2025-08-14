#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Zone1DrvSeatProxyImpl.hpp>

static int cmd_driveseat_get_pos (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone1::drvseat::control::Zone1DriverSeatProxyImpl::GetInstance();
	zone1::drvseat::control::EcuDrvseatMotorPosition motorPos;

	ss << "getterMotorPos (+)";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	bool ret = inst->getterMotorPos(motorPos);

	ss << "getterMotorPos (-) ret=" << (ret == true) ? "TRUE" : "FALSE";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "slidePos = " << (int)motorPos.slidePos;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "reclinePos = " << (int)motorPos.reclinePos;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "heightPos = " << (int)motorPos.heightPos;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "rlxTiltPos = " << (int)motorPos.rlxTiltPos;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	return 0;
}

static int cmd_driveseat_get_dir (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone1::drvseat::control::Zone1DriverSeatProxyImpl::GetInstance();
	zone1::drvseat::control::EcuDrvSeatMotorDirection motorDir;

	ss << "getterMotorDir (+)";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	bool ret = inst->getterMotorDir(motorDir);

	ss << "getterMotorDir (-) ret=" << (ret == true) ? "TRUE" : "FALSE";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "slide = " << (int)motorDir.slide;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "recline = " << (int)motorDir.recline;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "height = " << (int)motorDir.height;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "rlxTilt = " << (int)motorDir.rlxTilt;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "isNormal = " << (int)motorDir.isNormal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	return 0;
}

static int cmd_driveseat_motor (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone1::drvseat::control::Zone1DriverSeatProxyImpl::GetInstance();
	if (!inst->isServiceFound())
	{
		ss << "Zone1DriverSeat Service not found" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	if (argList.size() <= 0)
	{
		ss << "Usage: ecu motor [slide] [recline] [height] [rlxTilt]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	zone1::drvseat::control::EcuDrvSeatMotorInst command;

	command.slide = 0;
	command.recline = 0;
	command.height = 0;
	command.rlxTilt = 0;

	std::string arg = std::move(argList.front());
	argList.pop_front();

	int num;
	std::stringstream ss2(arg);
	ss2 >> num;
	if (ss2.fail())
	{
		ss << "Usage: ecu motor [slide] [recline] [height] [rlxTilt]" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	command.slide = (std::uint8_t)num;

	if (argList.size() > 0)
	{
		arg = std::move(argList.front());
		argList.pop_front();

		std::string arg2 = std::move(argList.front());
		argList.pop_front();
		std::stringstream ss3(arg2);
		ss3 >> num;
		if (!ss3.fail())
		{
			command.recline = (std::uint8_t)num;
		}
	}

	if (argList.size() > 0)
	{
		std::string arg2 = std::move(argList.front());
		argList.pop_front();
		std::stringstream ss3(arg2);
		ss3 >> num;
		if (!ss3.fail())
		{
			command.height = (std::uint8_t)num;
		}
	}

	if (argList.size() > 0)
	{
		std::string arg2 = std::move(argList.front());
		argList.pop_front();
		std::stringstream ss3(arg2);
		ss3 >> num;
		if (!ss3.fail())
		{
			command.rlxTilt = (std::uint8_t)num;
		}
	}

	inst->controlMotor(command);

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

	if (cmd == "motor")
	{
		return cmd_driveseat_motor(argList);
	}
	else if (cmd == "dir")
	{
		return cmd_driveseat_get_dir(argList);
	}
	else if (cmd == "pos")
	{
		return cmd_driveseat_get_pos(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [ecu]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

class DriveSeatListener : public zone1::drvseat::control::IZone1DrvseatListener
{
public:
	void notifyMotorDir(const zone1::drvseat::control::EcuDrvSeatMotorDirection &fieldValue) override
	{
		std::stringstream ss;
		ss << "notifyMotorDir (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorDirection slide = " << (int)fieldValue.slide;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorDirection recline = " << (int)fieldValue.recline;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorDirection height = " << (int)fieldValue.height;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorDirection rlxTilt = " << (int)fieldValue.rlxTilt;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorDirection isNormal = " << (int)fieldValue.isNormal;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "notifyMotorDir (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}

	void getMotorDir() override
	{

	}


	void notifyMotorPos(const zone1::drvseat::control::EcuDrvseatMotorPosition &fieldValue) override
	{
		std::stringstream ss;
		ss << "notifyMotorPos (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorPosition slidePos = " << (int)fieldValue.slidePos;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorPosition reclinePos = " << (int)fieldValue.reclinePos;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorPosition heightPos = " << (int)fieldValue.heightPos;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorPosition rlxTiltPos = " << (int)fieldValue.rlxTiltPos;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MotorPosition (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}

	void getMotorPos() override
	{

	}
};

void RegisterEcuApi()
{
	CmdMgr::GetInstance()->InsertCommand("ecu",
			"ecu [motor|dir|pos] (args...)",
			"debug for ECU Driver Seat APIs",
			"debug for ECU Driver Seat APIs",
			cmd_Ecu, nullptr);

	auto *inst = zone1::drvseat::control::Zone1DriverSeatProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<DriveSeatListener>());
}