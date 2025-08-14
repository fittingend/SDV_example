#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>

#include <VehicleContext.hpp>
#include <ApiMoodlamp_Local.hpp>

static CmdMgr *s_mgr = nullptr;

static void println_cmd (std::string string)
{
    if (s_mgr != nullptr)
    {
        std::stringstream ss;
        ss << string << "\r\n";
        s_mgr->Print(ss.str());
    }
}

static int cmd_moodlampColor (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: mlm color [color_index]\r\n";
        println_cmd(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();

    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: mlm color [color_index]\r\n";
        println_cmd(ss.str());
        return 0;
    }

    Api_Moodlamp_Method_RequestMlmSetRgbColor(num);
    return 0;
}

static int cmd_moodlampMode (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: mlm mode [care|comfort|delight|theraphy|vital]\r\n";
        println_cmd(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();

    MlmMoodMode_e mode;
    if (arg == "care")
    {
        mode = eMlmMoodMode_CareMood;
    }
    else if (arg == "comport")
    {
        mode = eMlmMoodMode_ComfortMood;

    }
    else if (arg == "delight")
    {
        mode = eMlmMoodMode_DelightMood;
    }
    else if (arg == "theraphy")
    {
        mode = eMlmMoodMode_MoodTherapy;
    }
    else if (arg == "vital")
    {
        mode = eMlmMoodMode_VitalityMood;
    }
    else
    {
        ss << "Usage: mlm mode [care|comfort|delight|theraphy|vital]\r\n";
        println_cmd(ss.str());
        return 0;
    }

    Api_Moodlamp_Method_RequestMlmSetMode(mode);
    return 0;
}

static int cmd_moodlampBright (std::list<std::string> &argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        ss << "Usage: mlm bright [brightness]\r\n";
        println_cmd(ss.str());
        return 0;
    }

    std::string arg = std::move(argList.front());
    argList.pop_front();

    int num;
    std::stringstream ss2(arg);
    ss2 >> num;
    if (ss2.fail())
    {
        ss << "Usage: mlm bright [brightness]\r\n";
        println_cmd(ss.str());
        return 0;
    }

    Api_Moodlamp_Method_RequestMlmSetBrightness(num);
    return 0;
}

static int cmd_MoodLamp (std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();

    if (cmd == "color")
    {
        return cmd_moodlampColor(argList);
    }
    else if (cmd == "mode")
    {
        return cmd_moodlampMode(argList);
    }
    else if (cmd == "bright")
    {
        return cmd_moodlampBright(argList);
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [mlm]";
        println_cmd(ss.str());
        return -1;
    }

    return 0;
}

void RegisterMoodLampCommands(CmdMgr *cmdMgr)
{
    if (s_mgr == nullptr)
    {
        s_mgr = cmdMgr;
    }

    cmdMgr->InsertCommand("mlm",
            "mlm [color/mode/bright] (args...)",
            "debug for Mood Lamp",
            "debug for Mood Lamp",
            cmd_MoodLamp);
}