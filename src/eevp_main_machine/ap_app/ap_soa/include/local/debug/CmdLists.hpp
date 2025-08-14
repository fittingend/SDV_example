#ifndef __CMD_LISTS_HPP__
#define __CMD_LISTS_HPP__

#include <CmdMgr.hpp>

// CmdLists_UdpParser.cpp
void RegisterUdpParserCommands(CmdMgr *cmdMgr);

// CmdLists_VehicleContetxt.cpp
void RegisterVehicleContextCommands(CmdMgr *cmdMgr);

// CmdLists_MoodLamp.cpp
void RegisterMoodLampCommands(CmdMgr *cmdMgr);

// CmdLists_Rcurtain.cpp
void RegisterRearCurtainCommands(CmdMgr *cmdMgr);

// CmdLists_UdpServer.cpp
void RegisterUdpServerCommands(CmdMgr *cmdMgr);

#endif