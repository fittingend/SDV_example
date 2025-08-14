#ifndef __CMD_LISTS_HPP__
#define __CMD_LISTS_HPP__

#include <string>
#include <list>

//void RegisterUdpServerCommands(CmdMgr *cmdMgr);
extern void RegisterSoaApi();
extern void RegisterCmdTimer();

extern int cmd_api_Dms(std::list<std::string> &argList);
extern int cmd_api_Hvac(std::list<std::string> &argList);
extern int cmd_api_SmartFilm (std::list<std::string> &argList);

#endif