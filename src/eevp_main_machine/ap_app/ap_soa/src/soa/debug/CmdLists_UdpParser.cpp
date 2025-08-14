#include <CmdMgr.hpp>
#include <CmdLists.hpp>
#include <sstream>
#include <string>
#include <string.h>

#include <VehicleContext.hpp>
#include <UdpParser.hpp>

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

static int cmd_TestUdpParser (std::list<std::string> argList)
{
    std::stringstream ss;
    if (argList.size() <= 0)
    {
        return -1;
    }

    std::string cmd = std::move(argList.front());
    argList.pop_front();
    UdpParser *parser = UdpParser::GetInstance();
    VehicleContext *context = VehicleContext::GetInstance();

    if (cmd == "test1")
    {
        ss << "The VehicleContext Before the parsing test binary 1:";
        println_cmd(ss.str());
        ss.clear();

        println_cmd(context->DebugInfo());

        unsigned char buffer[256];
        memset(buffer, 0xAA, 256);
        buffer[2] = 0x03;
        buffer[22] = 0x01;
        parser->ParseData(buffer);

        ss << "The VehicleContext After the parsing test binary 1:";
        println_cmd(ss.str());
        println_cmd(context->DebugInfo());
        ss.clear();
    }
    else
    {
        ss << "No Sub-Cmd(" << cmd << ") in the CMD [UdpParser]";
        println_cmd(ss.str());
        return -1;
    }

    return 0;
}

void RegisterUdpParserCommands(CmdMgr *cmdMgr)
{
    if (s_mgr == nullptr)
    {
        s_mgr = cmdMgr;
    }

    cmdMgr->InsertCommand("udp_parser",
            "udp_parser [test1] (args...)",
            "test udp parser",
            "test udp parser",
            cmd_TestUdpParser);

}