#include <Common.h>
#include <SoaDataType.h>
#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>
#include <random>
#include "SmartFilmProxyImpl.h"
#include <Debug_DataType2String.hpp>

static int cmd_api_smtfilm_auto (std::list<std::string> &argList)
{
    std::stringstream ss;
    static bool s_AutoMode = false;

    s_AutoMode = !s_AutoMode;
    ss << "Auto Mode => " << getStringEnum_bool(s_AutoMode);
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance()->SetAuto(s_AutoMode);
    return 0;
}

static int cmd_api_smtfilm_opacity (std::list<std::string> &argList)
{
    std::stringstream ss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);
    static int s_pos = 16;

    s_pos++;
    if (s_pos > (int)eSmtfilmPos_RearGrass)
    {
        s_pos = (int)eSmtfilmPos_WindShield;
    }
    else if (s_pos < (int)eSmtfilmPos_WindShield)
    {
        s_pos = (int)eSmtfilmPos_WindShield;
    }

    int opacity = dist(gen);

    ss << "Opacity[" << getStringEnum_SmtfilmPos_e((SmtfilmPos_e)s_pos) << "] => " << opacity;
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance()->SetOpacity((SmtfilmPos_e)s_pos, opacity);
    return 0;
}

static int cmd_api_smtfilm_status (std::list<std::string> &argList)
{
    std::stringstream ss;
    DeviceNormal_e isNormal;
    bool autoMode;
    SmtfilmOpacities_t opacities;
    int version;

    if (eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance()->getterIsDeviceNormal(isNormal))
    {
        ss << "- isNormal: " << getStringEnum_DeviceNormal_e(isNormal);
    }
    else
    {
        ss << "- getterIsDeviceNormal: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance()->getterSmartFilmAutoMode(autoMode))
    {
        ss << "- autoMode: " << getStringEnum_bool(autoMode);
    }
    else
    {
        ss << "- getterSmartFilmAutoMode: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance()->getterSmartFilmOpacities(opacities))
    {
        ss << "- opacities:\n  ";
        ss << getString_SmtfilmOpacities_t(opacities);
    }
    else
    {
        ss << "- getterHvacStatus: false";
    }
    CmdMgr::GetInstance()->Println(ss.str());
    ss.str("");

    if (eevp::control::smartfilm::SmartFilmProxyImpl::GetInstance()->getterSmartFilmSwVersion(version))
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

int cmd_api_SmartFilm (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: api smtfilm [auto|opacity|status] [options...]";
        CmdMgr::GetInstance()->Println(ss.str());
        return 0;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    std::for_each(cmd.begin(), cmd.end(), [](auto &c) { c = tolower(c); });

    if (cmd == "auto")
    {
         return cmd_api_smtfilm_auto(argList);
    }
    else if (cmd == "opacity")
    {
        return cmd_api_smtfilm_opacity(argList);
    }
    else if (cmd == "status")
    {
        return cmd_api_smtfilm_status(argList);
    }

    ss << "Usage: api smtfilm [auto|opacity|status] [options...] - subcmd [" << cmd << "] is not supported";
    CmdMgr::GetInstance()->Println(ss.str());

    return 0;
}