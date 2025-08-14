#ifndef __TELNET_SERVER_HPP__
#define __TELNET_SERVER_HPP__

#include <Common.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <list>
#include <string>
#include <functional>
#include <MsgSender.hpp>

class TelnetServer : public MsgSender
{
private:
	static TelnetServer *mInst;

	std::recursive_mutex mMutexForApi;

	int mClientSocket;
	int mServerSocket;
	bool mThreadCreated;
	std::atomic<bool> mThreadRunning;
	std::atomic<bool> mTelnetReady;
	std::atomic<bool> mDisconnectClient;

	std::mutex mMutexForUnsent;
	std::list<std::string> mUnsentStringBuffer;
	std::thread mThread;

	char mInputCmd[1024];
	int mInputPointer;

	TelnetServer();
	~TelnetServer();
	void RunServer(void);
	int CreateServer(void);
	int ConnectClient(void);
	int CommunicateWithClient(void);
	int ReadCommand(char buffer[], int length);
	void ReadyTelnet(void);
	int CleanResources(void);
	void ExecuteErrorState(void);


public:
	static TelnetServer *GetInstance();
	static void DestroyInstance();
	void Start(void);
	void Stop(void);
	void Disconnect(void);
	void SendMessage(std::string Message) override;
};

#endif