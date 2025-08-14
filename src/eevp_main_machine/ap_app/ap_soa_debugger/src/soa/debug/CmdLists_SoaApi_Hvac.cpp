#include <Common.h>
#include <SoaDataType.h>
#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>
#include "HvacProxyImpl.h"
#include <Debug_DataType2String.hpp>

static int cmd_api_hvac_power (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac power [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetHvacPower(true);
        ss << "- SetPower(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetHvacPower(false);
        ss << "- SetPower(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac power [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_ac (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac ac [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetAcOnOff(true);
        ss << "- SetAcOnOff(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetAcOnOff(false);
        ss << "- SetAcOnOff(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac ac [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_auto (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac auto [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetAutoTemp(true);
        ss << "- SetAutoTemp(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetAutoTemp(false);
        ss << "- SetAutoTemp(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac auto [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_sync (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac sync [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetSync(true);
        ss << "- SetSync(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetSync(false);
        ss << "- SetSync(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac sync [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_driver_temp (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac driver_temp [0 ~ 100]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: api hvac driver_temp [0 ~ 100]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    eevp::control::hvac::HvacProxyImpl::GetInstance()->SetDriverTemp((std::int8_t)num);
    ss << "- SetDriverTemp(" << num << ")";
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    return 0;
}

static int cmd_api_hvac_passenger_temp (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac passenger_temp [0 ~ 100]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: api hvac passenger_temp [0 ~ 100]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    eevp::control::hvac::HvacProxyImpl::GetInstance()->SetPassengerTemp((std::int8_t)num);
    ss << "- SetPassengerTemp(" << num << ")";
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    return 0;
}

static int cmd_api_hvac_blow_power (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac blow_power [0 ~ 100]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: api hvac blow_power [0 ~ 100]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBlowingForce((std::uint8_t)num);
    ss << "- SetBlowingForce(" << num << ")";
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    return 0;
}

static int cmd_api_hvac_blow_dir (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac blow_dir [chest|both|foot|foot_ws]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "chest")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBlowingDirection(eevp::control::SoaHvacBlowDir::kCHEST);
        ss << "- SetBlowingDirection(eevp::control::SoaHvacBlowDir::kCHEST)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "both")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBlowingDirection(eevp::control::SoaHvacBlowDir::kBOTH);
        ss << "- SetBlowingDirection(eevp::control::SoaHvacBlowDir::kBOTH)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "foot")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBlowingDirection(eevp::control::SoaHvacBlowDir::kFOOT);
        ss << "- SetBlowingDirection(eevp::control::SoaHvacBlowDir::kFOOT)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "foot_ws")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBlowingDirection(eevp::control::SoaHvacBlowDir::kFOOT_WITH_WS);
        ss << "- SetBlowingDirection(eevp::control::SoaHvacBlowDir::kFOOT_WITH_WS)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac blow_dir [chest|both|foot|foot_ws]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_air_src (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac air_src [inner|outer]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "outer")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetAirSource(eevp::control::SoaHvacAirSource::kOUTER);
        ss << "- SetAirSource(eevp::control::SoaHvacAirSource::kOUTER)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "inner")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetAirSource(eevp::control::SoaHvacAirSource::kINNER);
        ss << "- SetAirSource(eevp::control::SoaHvacAirSource::kINNER)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac air_src [inner|outer]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_fwddefrost (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac fwddefrost [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetForwardDefrostOn(true);
        ss << "- SetForwardDefrostOn(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetForwardDefrostOn(false);
        ss << "- SetForwardDefrostOn(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac fwddefrost [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_bckdefrost (std::list<std::string> &argList)
{
    std::stringstream ss;

    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac bckdefrost [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();
    std::for_each(arg.begin(), arg.end(), [](auto &c) { c = tolower(c); });

    if (arg == "on")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBackwardDefrostOn(true);
        ss << "- SetBackwardDefrostOn(true)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else if (arg == "off")
    {
        eevp::control::hvac::HvacProxyImpl::GetInstance()->SetBackwardDefrostOn(false);
        ss << "- SetBackwardDefrostOn(false)";
        CmdMgr::GetInstance()->Println(ss.str());
        ss.str("");
    }
    else
    {
        ss << "Usage: api hvac bckdefrost [on|off]";
        CmdMgr::GetInstance()->Println(ss.str());
    }

    return 0;
}

static int cmd_api_hvac_status (std::list<std::string> &argList)
{
    std::stringstream ss;
    DeviceNormal_e isNormal;
    HvacStatus_t setting;
    HvacStatus_t status;
    HvacTemps_t temps;
    int version;

    if (eevp::control::hvac::HvacProxyImpl::GetInstance()->getterIsDeviceNormal(isNormal))
    {
        ss << "- isNormal: " << getStringEnum_DeviceNormal_e(isNormal);
    }
    else
    {
        ss << "- getterIsDeviceNormal: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::hvac::HvacProxyImpl::GetInstance()->getterHvacSetting(setting))
    {
        ss << "- setting:\n  ";
        ss << getString_HvacStatus_t(setting);
    }
    else
    {
        ss << "- getterHvacSetting: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::hvac::HvacProxyImpl::GetInstance()->getterHvacStatus(status))
    {
        ss << "- status:\n  ";
        ss << getString_HvacStatus_t(status);
    }
    else
    {
        ss << "- getterHvacStatus: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::hvac::HvacProxyImpl::GetInstance()->getterHvacTemps(temps))
    {
        ss << "- temps:\n  ";
        ss << getString_HvacTemps_t(temps);
    }
    else
    {
        ss << "- getterHvacTemps: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::hvac::HvacProxyImpl::GetInstance()->getterHvacSwVersion(version))
    {
        ss << "- version=" << version;
    }
    else
    {
        ss << "- getterHvacSwVersion: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    return 0;
}

int cmd_api_Hvac (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api hvac [power|ac|auto|sync|driver_temp|passenger_temp|blow_power|blow_dir|air_src|fwddefrost|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "power")
    {
         return cmd_api_hvac_power(argList);
    }
    else if (cmd == "ac")
    {
        return cmd_api_hvac_ac(argList);
    }
    else if (cmd == "auto")
    {
        return cmd_api_hvac_auto(argList);
    }
    else if (cmd == "sync")
    {
        return cmd_api_hvac_sync(argList);
    }
    else if (cmd == "driver_temp")
    {
        return cmd_api_hvac_driver_temp(argList);
    }
    else if (cmd == "passenger_temp")
    {
        return cmd_api_hvac_passenger_temp(argList);
    }
    else if (cmd == "blow_power")
    {
        return cmd_api_hvac_blow_power(argList);
    }
    else if (cmd == "blow_dir")
    {
        return cmd_api_hvac_blow_dir(argList);
    }
    else if (cmd == "air_src")
    {
        return cmd_api_hvac_air_src(argList);
    }
    else if (cmd == "fwddefrost")
    {
        return cmd_api_hvac_fwddefrost(argList);
    }
    else if (cmd == "bckdefrost")
    {
        return cmd_api_hvac_bckdefrost(argList);
    }
    else if (cmd == "status")
    {
        return cmd_api_hvac_status(argList);
    }

    ss << "Usage: api hvac [power|ac|auto|sync|driver_temp|passenger_temp|blow_power|blow_dir|air_src|fwddefrost|status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}