#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>

#include <VehicleContext.hpp>

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

static int cmd_loadVehicleContext (std::list<std::string> &argList)
{

    return 0;
}

static int cmd_saveVehicleContext (std::list<std::string> &argList)
{

    return 0;
}

static int cmd_printVehicleContext (std::list<std::string> &argList)
{
    VehicleContext *context = VehicleContext::GetInstance();
    println_cmd(context->DebugInfo());
    return 0;
}

static int cmd_VehicleContext (std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();

    if (cmd == "load")
    {
        return cmd_loadVehicleContext(argList);
    }
    else if (cmd == "save")
    {
        return cmd_saveVehicleContext(argList);
    }
    else if (cmd == "print")
    {
        return cmd_printVehicleContext(argList);
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [vc]";
        println_cmd(ss.str());
        return -1;
    }

    return 0;
}

void RegisterVehicleContextCommands(CmdMgr *cmdMgr)
{
    if (s_mgr == nullptr)
    {
        s_mgr = cmdMgr;
    }

    cmdMgr->InsertCommand("vc",
            "vc [load/save/print] (args...)",
            "debug for vehicle context",
            "debug for vehicle context",
            cmd_VehicleContext);

}