#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>

#include "MoodLampProxyImpl.h"
#include "RearCurtainProxyImpl.h"
#include "DriverSeatProxyImpl.h"
#include "RoaProxyImpl.h"
#include "WiperProxyImpl.h"
#include "PowerProxyImpl.h"
#include "DmsProxyImpl.h"
#include "HvacProxyImpl.h"
#include "SmartFilmProxyImpl.h"

//
//  Mood Lamp APIs
//
static int cmd_api_moodlamp_color (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api mlm color [color_index (int)]";
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
        ss << "Usage: api mlm color [color_index (int)]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    auto *inst = eevp::control::moodlamp::MoodLampProxyImpl::GetInstance();
    inst->requestMlmSetRGBColor(num);
    return 0;
}

static int cmd_api_moodlamp_mode (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api mlm mode [care|comfort|delight|theraphy|vital]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();

    eevp::control::SoaMlmMoodMode mode;
    if (arg == "care")
    {
        mode = eevp::control::SoaMlmMoodMode::kCARE_MOOD;
    }
    else if (arg == "comport")
    {
        mode = eevp::control::SoaMlmMoodMode::kCOMPORT_MOOD;
    }
    else if (arg == "delight")
    {
        mode = eevp::control::SoaMlmMoodMode::kDELIGHT_MOOD;
    }
    else if (arg == "theraphy")
    {
        mode = eevp::control::SoaMlmMoodMode::kMOOD_THERAPHY;
    }
    else if (arg == "vital")
    {
        mode = eevp::control::SoaMlmMoodMode::kVITALITY_MOOD;
    }
    else
    {
        ss << "Usage: api mlm mode [care|comfort|delight|theraphy|vital] - no mode with name [" << arg << "]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    auto *inst = eevp::control::moodlamp::MoodLampProxyImpl::GetInstance();
    inst->requestMlmSetMode(mode);

    return 0;
}

static int cmd_api_moodlamp_bright(std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api mlm bright [brightness (int)]";
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
        ss << "Usage: api mlm bright [brightness (int)]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    auto *inst = eevp::control::moodlamp::MoodLampProxyImpl::GetInstance();
    inst->requestMlmSetBrightness(num);
    return 0;
}

static int cmd_api_moodlamp_status (std::list<std::string> &argList)
{
    eevp::control::SoaMlmStatus soaMlmStatus;
    std::stringstream ss;
    auto *inst = eevp::control::moodlamp::MoodLampProxyImpl::GetInstance();
    bool ret = inst->getterSoaMlmStatus(soaMlmStatus);
    if (!ret)
    {
        ss << "getterSoaMlmStatus return false\r\n";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    ss << "getterSoaMlmStatus return true\r\n";
    ss << "   colorTableIndex = " << (int)soaMlmStatus.colorTableIndex << "\r\n";
    ss << "   brightness = " << (int)soaMlmStatus.brightness << "\r\n";
    ss << "   isNormal = " << (int)soaMlmStatus.isNormal << "\r\n";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_moodlamp (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api mlm [color|mode|bright|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "color")
    {
        return cmd_api_moodlamp_color(argList);
    }
    else if (cmd == "mode")
    {
        return cmd_api_moodlamp_mode(argList);
    }
    else if (cmd == "bright")
    {
        return cmd_api_moodlamp_bright(argList);
    }
    else if (cmd == "status")
    {
        return cmd_api_moodlamp_status(argList);
    }

    ss << "Usage: api mlm [color|mode|bright|status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

//
//  Rear Curtain APIs
//

static int cmd_api_rearcurtain_op (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api rctn op [up|down|stop]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    eevp::control::SoaRctnMotorDir dir;
    if (cmd == "up")
    {
        dir = eevp::control::SoaRctnMotorDir::kUP;
    }
    else if (cmd == "down")
    {
        dir = eevp::control::SoaRctnMotorDir::kDOWN;
    }
    else if (cmd == "stop")
    {
        dir = eevp::control::SoaRctnMotorDir::kSTOP;
    }
    else
    {
        ss << "Usage: api rctn op [up|down|stop] - argument [" << cmd << "] is not supported.";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    auto *inst = eevp::control::rearcurtain::RearCurtainProxyImpl::GetInstance();
    eevp::control::SoaErrorState err = inst->requestRearCurtainOperation(dir);

    ss << "Function return value = " << (int)err;
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_rearcurtain_status (std::list<std::string> &argList)
{
    eevp::control::SoaRctnStatus soaRctnStatus;
    std::stringstream ss;
    auto *inst = eevp::control::rearcurtain::RearCurtainProxyImpl::GetInstance();
    bool ret = inst->getterSoaRctnStatus(soaRctnStatus);
    if (!ret)
    {
        ss << "getterSoaRctnStatus return false";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    ss << "getterSoaRctnStatus return true\r\n";
    ss << "   errorState = " << (int)soaRctnStatus.errorState << "\r\n";
    ss << "   curMotorDirection = " << (int)soaRctnStatus.curMotorDirection << "\r\n";
    ss << "   curtainState = " << (int)soaRctnStatus.curtainState << "\r\n";
    ss << "   isNormal = " << (int)soaRctnStatus.isNormal << "\r\n";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_rearcurtain (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api rctn [op|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "op")
    {
        return cmd_api_rearcurtain_op(argList);
    }
    else if (cmd == "status")
    {
        return cmd_api_rearcurtain_status(argList);
    }

    ss << "Usage: api rctn [op|status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

//
//  Driver Seat APIs
//
static int cmd_api_driverseat_status (std::list<std::string> &argList)
{
    std::stringstream ss;
    auto *inst = eevp::control::driverseat::DriverSeatProxyImpl::GetInstance();

    eevp::control::SoaDeviceIsNormal isDeviceNormal;
    bool ret = inst->getterIsDeviceNormal(isDeviceNormal);
    if (ret)
    {
        ss << "- isDeviceNormal: " << (int)isDeviceNormal;
    }
    else
    {
        ss << "getterIsDeviceNormal return false";
    }

    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_driverseat_height (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api dseat height [up|down|stop]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    auto *inst = eevp::control::driverseat::DriverSeatProxyImpl::GetInstance();

    if (cmd == "up")
    {
        inst->RequestMoveDrvSeatHeight(eevp::control::SoaSeatHeightDirection::kUP);
        return 0;
    }
    else if (cmd == "down")
    {
        inst->RequestMoveDrvSeatHeight(eevp::control::SoaSeatHeightDirection::kDOWN);
        return 0;
    }
    else if (cmd == "stop")
    {
        inst->RequestMoveDrvSeatHeight(eevp::control::SoaSeatHeightDirection::kSTOP);
        return 0;
    }

    ss << "Usage: api dseat height [up|down|stop]";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_driverseat_recline (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api dseat recline [fwd|bwd|stop]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    auto *inst = eevp::control::driverseat::DriverSeatProxyImpl::GetInstance();

    if (cmd == "fwd")
    {
        inst->RequestMoveDrvSeatRecline(eevp::control::SoaSeatReclineDirection::kFORWARD);
        return 0;
    }
    else if (cmd == "bwd")
    {
        inst->RequestMoveDrvSeatRecline(eevp::control::SoaSeatReclineDirection::kBACKWARD);
        return 0;
    }
    else if (cmd == "stop")
    {
        inst->RequestMoveDrvSeatRecline(eevp::control::SoaSeatReclineDirection::kSTOP);
        return 0;
    }

    ss << "Usage: api dseat height [up|down|stop]";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_driverseat (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api dseat [status|height|recline] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "status")
    {
        return cmd_api_driverseat_status(argList);
    }
    else if (cmd == "height")
    {
        return cmd_api_driverseat_height(argList);
    }
    else if (cmd == "recline")
    {
        return cmd_api_driverseat_recline(argList);
    }

    ss << "Usage: api dseat [status|height|recline] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

//
//  ROA APIs
//

static int cmd_api_power (std::list<std::string> &argList)
{
    std::stringstream ss;
#if 0
    if (argList.size() <= 0)
    {
        ss << "Usage: api dseat [status|height|recline] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });
#endif
    auto *inst = eevp::control::power::PowerProxyImpl::GetInstance();
    eevp::control::SoaDeviceIsNormal isDeviceNormal;
    bool ret = inst->getterSoaPowerDeviceNormal(isDeviceNormal);
    if (ret)
    {
        ss << "- isDeviceNormal: " << (int)isDeviceNormal;
    }
    else
    {
        ss << "getterIsDeviceNormal return false";
    }

    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

//
//  ROA APIs
//
static int cmd_api_roa_start (std::list<std::string> &argList)
{
    std::stringstream ss;
    auto *inst = eevp::control::roa::RoaProxyImpl::GetInstance();

    bool ret = inst->StartRoa();
    if (ret)
    {
        ss << "eevp.control.SoaRoa.StartRoa called:";
    }
    else
    {
        ss << "eevp.control.SoaRoa.StartRoa failed:";
    }

    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_roa_stop (std::list<std::string> &argList)
{
    std::stringstream ss;
    auto *inst = eevp::control::roa::RoaProxyImpl::GetInstance();

    bool ret = inst->StopRoa();
    if (ret)
    {
        ss << "eevp.control.SoaRoa.StopRoa called:";
    }
    else
    {
        ss << "eevp.control.SoaRoa.StopRoa failed:";
    }

    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_roa_status (std::list<std::string> &argList)
{
    std::stringstream ss;
    auto *inst = eevp::control::roa::RoaProxyImpl::GetInstance();

    eevp::control::SoaDeviceIsNormal isDeviceNormal;
    bool ret = inst->getterIsDeviceNormal(isDeviceNormal);
    if (ret)
    {
        ss << "- isDeviceNormal: " << (int)isDeviceNormal;
    }
    else
    {
        ss << "getterIsDeviceNormal return false";
    }

    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_roa (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api roa [start|stop|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "status")
    {
        return cmd_api_roa_status(argList);
    }
    else if (cmd == "start")
    {
        return cmd_api_roa_start(argList);
    }
    else if (cmd == "stop")
    {
        return cmd_api_roa_stop(argList);
    }

    ss << "Usage: api roa [start|stop|status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

//
//  Wiper APIs
//

static int cmd_api_wiper_op (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api wiper op [on|off|int1|int2|int3]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    eevp::control::SoaWiperMode wiperMode;

    if (cmd == "off")
    {
       wiperMode = eevp::control::SoaWiperMode::kOFF;
    }
    else if (cmd == "on")
    {
       wiperMode = eevp::control::SoaWiperMode::kON;
    }
    else if (cmd == "int1")
    {
       wiperMode = eevp::control::SoaWiperMode::kINT1;
    }
    else if (cmd == "int2")
    {
       wiperMode = eevp::control::SoaWiperMode::kINT2;
    }
    else if (cmd == "int3")
    {
       wiperMode = eevp::control::SoaWiperMode::kINT3;
    }
    else
    {
        ss << "Usage: api wiper op [on|off|int1|int2|int3]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    auto *inst = eevp::control::wiper::WiperProxyImpl::GetInstance();
    inst->RequestWiperOperation(wiperMode);

    return 0;
}

static int cmd_api_wiper_status (std::list<std::string> &argList)
{
    std::stringstream ss;
    auto *inst = eevp::control::wiper::WiperProxyImpl::GetInstance();

    eevp::control::SoaDeviceIsNormal isDeviceNormal;
    bool ret = inst->getterIsDeviceNormal(isDeviceNormal);
    if (ret)
    {
        ss << "- isDeviceNormal: " << (int)isDeviceNormal;
    }
    else
    {
        ss << "getterIsDeviceNormal return false";
    }

    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_wiper (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api wiper [op|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "status")
    {
        return cmd_api_wiper_status(argList);
    }
    else if (cmd == "op")
    {
        return cmd_api_wiper_op(argList);
    }

    ss << "Usage: api wiper [op|status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());
    return 0;
}

static int cmd_api_subscribe (std::list<std::string> &argList)
{
    auto *drvseat_inst = eevp::control::driverseat::DriverSeatProxyImpl::GetInstance();
    auto *moddlamp_inst = eevp::control::moodlamp::MoodLampProxyImpl::GetInstance();
    auto *power_inst = eevp::control::power::PowerProxyImpl::GetInstance();
    auto *rcurtain_inst = eevp::control::rearcurtain::RearCurtainProxyImpl::GetInstance();
    auto *roa_inst = eevp::control::roa::RoaProxyImpl::GetInstance();
    auto *wiper_inst = eevp::control::wiper::WiperProxyImpl::GetInstance();
    auto *dms_inst = eevp::control::dms::DmsProxyImpl::GetInstance();
    auto *hvac_inst = eevp::control::hvac::HvacProxyImpl::GetInstance();
    auto *smtfilm_inst = eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance();

    CmdMgr::GetInstance()->Println(drvseat_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(moddlamp_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(power_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(rcurtain_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(roa_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(wiper_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(dms_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(hvac_inst->GetSubscriptStatusString());
    CmdMgr::GetInstance()->Println(smtfilm_inst->GetSubscriptStatusString());

    return 0;
}

//
//  Root CMD Functions
//

static int cmd_SoaApis(void *item, std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "mlm")
    {
        return cmd_api_moodlamp(argList);
    }
    else if (cmd == "rctn")
    {
        return cmd_api_rearcurtain(argList);
    }
    else if (cmd == "dseat")
    {
        return cmd_api_driverseat(argList);
    }
    else if (cmd == "power")
    {
        return cmd_api_power(argList);
    }
    else if (cmd == "roa")
    {
        return cmd_api_roa(argList);
    }
    else if (cmd == "wiper")
    {
        return cmd_api_wiper(argList);
    }
    else if (cmd == "dms")
    {
        return cmd_api_Dms(argList);
    }
    else if (cmd == "hvac")
    {
        return cmd_api_Hvac(argList);
    }
    else if (cmd == "smtfilm")
    {
        return cmd_api_SmartFilm(argList);
    }
    else if (cmd == "subscribe")
    {
        return cmd_api_subscribe(argList);
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [api]";
        CmdMgr::GetInstance()->Println(ss.str());
        return -1;
    }

    return 0;
}

void RegisterSoaApi()
{
    CmdMgr::GetInstance()->InsertCommand("api",
            "api [mlm|rctn|dseat|roa|dms|hvac|smtfilm|subscribe] (args...)",
            "debug for SOA APIs",
            "debug for SOA APIs",
            cmd_SoaApis, nullptr);
}