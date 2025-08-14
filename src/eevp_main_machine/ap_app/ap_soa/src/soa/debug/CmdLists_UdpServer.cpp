#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>

#include <VehicleContext.hpp>
#include <UdpServer.hpp>

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

static int cmd_SetUdpServerLog (std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    UdpServer *server = UdpServer::GetInstance();
    VehicleContext *context = VehicleContext::GetInstance();

    if (cmd == "log")
    {
        if (argList.size() < 2)
        {
            ss << "[usage] udp_server log [input|output] [1|0]";
            println_cmd(ss.str());
            ss.clear();
            return -1;
        }

        std::string log_type = std::move(argList.front());
        argList.pop_front();
        std::string on_off = std::move(argList.front());
        argList.pop_front();

        bool flag = (on_off == "1");
        if (log_type == "input")
        {
            server->SetReceivedDataPrint(flag);
        }
        else if (log_type == "output")
        {
            server->SetSentDataPrint(flag);
        }
        else
        {
            ss << "[usage] udp_server log [input|output] [1|0]";
            println_cmd(ss.str());
            ss.clear();
            return -1;
        }
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [UdpParser]";
        println_cmd(ss.str());
        return -1;
    }

    return 0;
}

void RegisterUdpServerCommands(CmdMgr *cmdMgr)
{
    if (s_mgr == nullptr)
    {
        s_mgr = cmdMgr;
    }

    cmdMgr->InsertCommand("udp_server",
            "udp_server [log] (args...)",
            "test udp server",
            "test udp server",
            cmd_SetUdpServerLog);

}