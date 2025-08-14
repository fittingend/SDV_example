#ifndef __TELNET_SERVER_HPP__
#define __TELNET_SERVER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <thread>
#include <atomic>
#include <functional>
#include <CmdMgr.hpp>
#include <MsgSender.hpp>

class TelnetServer : public MsgSender
{
private:
    int mClientSocket;
    int mServerSocket;
    bool mThreadCreated;
    std::atomic<bool> mThreadRunning;
    std::atomic<bool> mTelnetReady;
    std::thread mThread;

    char mInputCmd[1024];
    int mInputPointer;

    CmdMgr *mCmdMgr;

    void RunServer(void);
    int CreateServer(void);
    int ConnectClient(void);
    int CommunicateWithClient(void);
    int ReadCommand(char buffer[], int length);
    void ReadyTelnet(void);
    int CleanResources(void);
    void ExecuteErrorState(void);


public:
    TelnetServer();
    ~TelnetServer();
    void Start(void);
    void Stop(void);
    void SendMessage(std::string Message) override;
};

#endif