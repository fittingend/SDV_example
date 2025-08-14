#include <Common.h>
#include <SoaDataType.h>
#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>
#include "DmsProxyImpl.h"
#include <Debug_DataType2String.hpp>

static int cmd_api_dms_power (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api dms power [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::dms::DmsProxyImpl::GetInstance()->SetPower(true);
        ss << "- SetPower(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::dms::DmsProxyImpl::GetInstance()->SetPower(false);
        ss << "- SetPower(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api dms power [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_dms_status (std::list<std::string> &argList)
{
    std::stringstream ss;
    auto *inst = eevp::control::dms::DmsProxyImpl::GetInstance();

    {
        DeviceNormal_e isDeviceNormal;
        bool ret = inst->getterIsDeviceNormal(isDeviceNormal);
        if (ret)
        {
            ss << "- isDeviceNormal: " << getStringEnum_DeviceNormal_e(isDeviceNormal);
        }
        else
        {
            ss << "getterIsDeviceNormal return false";
        }

        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }

    {
        bool powerOn;
        DmsDriverInside_e inside;
        bool ret = inst->getterDmsStatus(powerOn, inside);
        if (ret)
        {
            ss << "- powerOn: " << getStringEnum_bool(powerOn) << "\n";
            ss << "- inside: " << getStringEnum_DmsDriverInside_e(inside);
        }
        else
        {
            ss << "getterDmsStatus return false";
        }

        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }

    {
        DmsFatigue_e fatigue;
        DmsGazingDir_e dir;
        DmsOnPhone_e phone;
        DmsSmoking_e smoking;
        bool ret = inst->getterDmsDriverStatus(fatigue, dir, phone, smoking);
        if (ret)
        {
            ss << "- fatigue: " << getStringEnum_DmsFatigue_e(fatigue) << "\n";
            ss << "- dir: " << getStringEnum_DmsGazingDir_e(dir) << "\n";
            ss << "- phone: " << getStringEnum_DmsOnPhone_e(phone) << "\n";
            ss << "- smoking: " << getStringEnum_DmsSmoking_e(smoking);
        }
        else
        {
            ss << "getterDmsDriverStatus return false";
        }

        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }


    return 0;
}

int cmd_api_Dms (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api dms [power|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "power")
    {
         return cmd_api_dms_power(argList);
    }
    else if (cmd == "status")
    {
        return cmd_api_dms_status(argList);
    }

    ss << "Usage: api dms [status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}