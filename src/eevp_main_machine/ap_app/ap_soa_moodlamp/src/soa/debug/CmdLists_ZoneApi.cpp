#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Zone1MlmProxyImpl.hpp>

static int cmd_driveseat_get_pos (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone1::mlm::control::Zone1MlmProxyImpl::GetInstance();
	zone1::mlm::control::EcuMlmState mlmState;

	ss << "getterMlmState (+)";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	bool ret = inst->getterMlmStatus(mlmState);

	ss << "getterMlmState (-) ret=" << (ret == true) ? "TRUE" : "FALSE";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "colorIndex = " << (int)mlmState.colorIndex;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "brightness = " << (int)mlmState.brightness;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	ss << "isNormal = " << (int)mlmState.isNormal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");
	return 0;
}

static int cmd_mlm_color (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone1::mlm::control::Zone1MlmProxyImpl::GetInstance();
	if (!inst->isServiceFound())
	{
		ss << "Zone1MoodLamp Service not found" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	if (argList.size() <= 0)
	{
		ss << "Usage: ecu color [colorIdx]";
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
		ss << "Usage: ecu color [colorIdx]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	inst->setMlmColor((std::uint8_t)num);

	return 0;
};

static int cmd_mlm_bright (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst = zone1::mlm::control::Zone1MlmProxyImpl::GetInstance();
	if (!inst->isServiceFound())
	{
		ss << "Zone1MoodLamp Service not found" ;
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	if (argList.size() <= 0)
	{
		ss << "Usage: ecu color [colorIdx]";
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
		ss << "Usage: ecu bright [value]";
		CmdMgr::GetInstance()->Println(ss.str());
		return 0;
	}

	inst->setMlmBrightness((std::uint8_t)num);

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

	if (cmd == "color")
	{
		return cmd_mlm_color(argList);
	}
	else if (cmd == "bright")
	{
		return cmd_mlm_bright(argList);
	}
	else if (cmd == "status")
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

class MoodLampListener : public zone1::mlm::control::IZone1MlmListener
{
public:
	void notifyMlmStatus(const zone1::mlm::control::EcuMlmState &fieldValue)
	{
		std::stringstream ss;
		ss << "notifyMlmStatus (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MlmStatus colorIndex = " << (int)fieldValue.colorIndex;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MlmStatus brightness = " << (int)fieldValue.brightness;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "MlmStatus isNormal = " << (int)fieldValue.isNormal;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "notifyMlmStatus (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}

	void getMlmStatus()
	{

	}
};

void RegisterEcuApi()
{
	CmdMgr::GetInstance()->InsertCommand("ecu",
			"ecu [color|bright|status] (args...)",
			"debug for ECU MOODLAMP APIs",
			"debug for ECU MOODLAMP APIs",
			cmd_Ecu, nullptr);

	auto *inst = zone1::mlm::control::Zone1MlmProxyImpl::GetInstance();
	inst->setEventListener(std::make_shared<MoodLampListener>());
}