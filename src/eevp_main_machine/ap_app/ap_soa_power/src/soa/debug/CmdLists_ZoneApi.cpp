#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <string>
#include <string.h>
#include <Zone1ReservedAProxyImpl.hpp>
#include <Zone2ReservedAProxyImpl.hpp>
#include <Zone3ReservedAProxyImpl.hpp>
#include <Zone4ReservedAProxyImpl.hpp>

static int cmd_driveseat_device (std::list<std::string> &argList)
{
	std::stringstream ss;

	auto *inst1 = zone1::reserved::a::Zone1ReservedAProxyImpl::GetInstance();
	auto *inst2 = zone2::reserved::a::Zone2ReservedAProxyImpl::GetInstance();
	auto *inst3 = zone3::reserved::a::Zone3ReservedAProxyImpl::GetInstance();
	auto *inst4 = zone4::reserved::a::Zone4ReservedAProxyImpl::GetInstance();

	uint8_t normal;
	bool ret;

	ret = inst1->getterDeviceNormal(normal);
	ss << "Zone1 Device (" << ((ret) ? "SVC" : "NOSVC") << ") - " << (int)normal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ret = inst2->getterDeviceNormal(normal);
	ss << "Zone2 Device (" << ((ret) ? "SVC" : "NOSVC") << ") - " << (int)normal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ret = inst3->getterDeviceNormal(normal);
	ss << "Zone3 Device (" << ((ret) ? "SVC" : "NOSVC") << ") - " << (int)normal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ret = inst4->getterDeviceNormal(normal);
	ss << "Zone4 Device (" << ((ret) ? "SVC" : "NOSVC") << ") - " << (int)normal;
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

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

	if (cmd == "device")
	{
		return cmd_driveseat_device(argList);
	}
	else
	{
		ss << "No Sub-Cmd(" << cmd << ") in the CMD [ecu]";
		CmdMgr::GetInstance()->Println(ss.str());
		return -1;
	}

	return 0;
}

class Zone1Linstener : public zone1::reserved::a::IZone1ReservedAListener
{
public:
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		std::stringstream ss;
		ss << "Zone1DeviceNormal (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone1DeviceNormal DeviceNormal = " << (int)fieldValue;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone1DeviceNormal (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}
};

class Zone2Linstener : public zone2::reserved::a::IZone2ReservedAListener
{
public:
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		std::stringstream ss;
		ss << "Zone2DeviceNormal (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone2DeviceNormal DeviceNormal = " << (int)fieldValue;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone2DeviceNormal (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}
};

class Zone3Linstener : public zone3::reserved::a::IZone3ReservedAListener
{
public:
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		std::stringstream ss;
		ss << "Zone3DeviceNormal (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone3DeviceNormal DeviceNormal = " << (int)fieldValue;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone3DeviceNormal (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}
};

class Zone4Linstener : public zone4::reserved::a::IZone4ReservedAListener
{
public:
	void notifyDeviceNormal(const std::uint8_t &fieldValue)
	{
		std::stringstream ss;
		ss << "Zone4DeviceNormal (+)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone4DeviceNormal DeviceNormal = " << (int)fieldValue;
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");

		ss << "Zone4DeviceNormal (-)";
		CmdMgr::GetInstance()->Println(ss.str());
		ss.str("");
	}
};

void RegisterEcuApi()
{
	CmdMgr::GetInstance()->InsertCommand("ecu",
			"ecu [device] (args...)",
			"debug for ECU Power APIs",
			"debug for ECU Power APIs",
			cmd_Ecu, nullptr);

	auto *inst1 = zone1::reserved::a::Zone1ReservedAProxyImpl::GetInstance();
	inst1->setEventListener(std::make_shared<Zone1Linstener>());

	auto *inst2 = zone2::reserved::a::Zone2ReservedAProxyImpl::GetInstance();
	inst2->setEventListener(std::make_shared<Zone2Linstener>());

	auto *inst3 = zone3::reserved::a::Zone3ReservedAProxyImpl::GetInstance();
	inst3->setEventListener(std::make_shared<Zone3Linstener>());

	auto *inst4 = zone4::reserved::a::Zone4ReservedAProxyImpl::GetInstance();
	inst4->setEventListener(std::make_shared<Zone4Linstener>());
}