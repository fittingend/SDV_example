#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>

#include <VehicleContext.hpp>
#include <ApiRcurtain_Local.hpp>

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

static int cmd_RearCurtain(std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();

    if (cmd == "open")
    {
        Api_Rcurtain_Method_RequestRearCurtainOperation(eRCtnSwitch_OpenOn);
    }
    else if (cmd == "close")
    {
        Api_Rcurtain_Method_RequestRearCurtainOperation(eRCtnSwitch_CloseOn);
    }
    else if (cmd == "stop")
    {
        Api_Rcurtain_Method_RequestRearCurtainOperation(eRCtnSwitch_CloseOn);
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [rcurtain]";
        println_cmd(ss.str());
        return -1;
    }

    return 0;
}

void RegisterRearCurtainCommands(CmdMgr *cmdMgr)
{
    if (s_mgr == nullptr)
    {
        s_mgr = cmdMgr;
    }

    cmdMgr->InsertCommand("rcurtain",
            "rcurtain [open/close/stop]",
            "debug for Rear Curtain",
            "debug for Rear Curtain",
            cmd_RearCurtain);
}