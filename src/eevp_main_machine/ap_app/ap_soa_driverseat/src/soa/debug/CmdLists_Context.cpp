#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <algorithm>
#include <sstream>
#include <regex>
#include <string>
#include <string.h>
#include <Api_SoaDriverSeat.hpp>
#include <Api_Zone1DrvSeat.hpp>
#include <Debug_DataType2String.hpp>
#include <VehicleContext.hpp>

static int cmd_Context_change_isnormal (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "DeviceNormal changes [" << getStringEnum_DeviceNormal_e(context->mDeviceNormal) << "] >> [";

	switch (context->mDeviceNormal)
	{
	case eDeviceNormal_Ok:
		context->mDeviceNormal = eDeviceNormal_Abnormal;
		break;
	case eDeviceNormal_Abnormal:
	default:
		context->mDeviceNormal = eDeviceNormal_Ok;
		break;
	}

	ss << getStringEnum_DeviceNormal_e(context->mDeviceNormal) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_Drvseat_UpdateDeviceNormal();
	return 0;
};

static int cmd_Context_change_dir (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Slide Direction changes [" << getStringEnum_DriverSeatDir_e(context->mDirSlide) << "] >> [";
	switch (context->mDirSlide)
	{
	case eDriverSeatDir_Stop:
		context->mDirSlide = eDriverSeatDir_Forward;
		break;
	case eDriverSeatDir_Forward:
		context->mDirSlide = eDriverSeatDir_Backward;
		break;
	case eDriverSeatDir_Backward:
	default:
		context->mDirSlide = eDriverSeatDir_Stop;
		break;
	}
	ss << getStringEnum_DriverSeatDir_e(context->mDirSlide) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "Recline Direction changes [" << getStringEnum_DriverSeatDir_e(context->mDirRecline) << "] >> [";
	switch (context->mDirRecline)
	{
	case eDriverSeatDir_Stop:
		context->mDirRecline = eDriverSeatDir_Forward;
		break;
	case eDriverSeatDir_Forward:
		context->mDirRecline = eDriverSeatDir_Backward;
		break;
	case eDriverSeatDir_Backward:
	default:
		context->mDirRecline = eDriverSeatDir_Stop;
		break;
	}
	ss << getStringEnum_DriverSeatDir_e(context->mDirRecline) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "Height Direction changes [" << getStringEnum_DriverSeatDir_e(context->mDirHeight) << "] >> [";
	switch (context->mDirHeight)
	{
	case eDriverSeatDir_Stop:
		context->mDirHeight = eDriverSeatDir_Up;
		break;
	case eDriverSeatDir_Up:
		context->mDirHeight = eDriverSeatDir_Down;
		break;
	case eDriverSeatDir_Down:
	default:
		context->mDirHeight = eDriverSeatDir_Stop;
		break;
	}
	ss << getStringEnum_DriverSeatDir_e(context->mDirRecline) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "Tilt Direction changes [" << getStringEnum_DriverSeatDir_e(context->mDirTilt) << "] >> [";
	switch (context->mDirTilt)
	{
	case eDriverSeatDir_Stop:
		context->mDirTilt = eDriverSeatDir_Up;
		break;
	case eDriverSeatDir_Up:
		context->mDirTilt = eDriverSeatDir_Down;
		break;
	case eDriverSeatDir_Down:
	default:
		context->mDirTilt = eDriverSeatDir_Stop;
		break;
	}
	ss << getStringEnum_DriverSeatDir_e(context->mDirTilt) << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_Drvseat_UpdateMotorDirection();
	return 0;
};

static int cmd_Context_change_pos (std::list<std::string> &argList)
{
	std::stringstream ss;
	VehicleContext *context = VehicleContext::GetInstance();

	ss << "Slide Position changes [" << context->mPosSlide << "] >> [";
	context->mPosSlide = (context->mPosSlide < 90) ? (context->mPosSlide + 1) : 0;
	ss << context->mPosSlide << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "Recline Position changes [" << context->mPosRecline << "] >> [";
	context->mPosRecline = (context->mPosRecline < 90) ? (context->mPosRecline + 1) : 0;
	ss << context->mPosRecline << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "Height Position changes [" << context->mPosHeight << "] >> [";
	context->mPosHeight = (context->mPosHeight < 90) ? (context->mPosHeight + 1) : 0;
	ss << context->mPosHeight << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	ss << "Tilt Position changes [" << context->mPosTilt << "] >> [";
	context->mPosTilt = (context->mPosTilt < 90) ? (context->mPosTilt + 1) : 0;
	ss << context->mPosTilt << "]";
	CmdMgr::GetInstance()->Println(ss.str());
	ss.str("");

	SOA_Drvseat_UpdateMotorPosition();
	return 0;
};

static int cmd_Context_change (std::list<std::string> &argList)
{
	std::stringstream ss;

	if (argList.size() <= 0)
	{
		ss << "Usage: context change [isnormal|dir|pos]";
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
	else if (arg == "dir")
	{
		return cmd_Context_change_dir(argList);
	}
	else if (arg == "pos")
	{
		return cmd_Context_change_pos(argList);
	}

	ss << "Usage: context change [isnormal|dir|pos]";
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_reset (std::list<std::string> &argList)
{
	std::stringstream ss;

	Zone1_Drvseat_UpdateContext();
	ss << "Context updated to the ECU zone1." ;
	CmdMgr::GetInstance()->Println(ss.str());
	return 0;
};

static int cmd_Context_Status (std::list<std::string> &argList)
{
	VehicleContext *context = VehicleContext::GetInstance();
	std::string context_text = VehicleContext::GetInstance()->DebugInfo();
	context_text = std::regex_replace(context_text, std::regex("\n"), "\r\n");
	CmdMgr::GetInstance()->Println(context_text);
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
	else if (cmd == "reset")
	{
		return cmd_Context_reset(argList);
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
			"context [status|change|reset] (args...)",
			"debug for Context",
			"debug for Context",
			cmd_Context, nullptr);
}