#ifndef ___MULTICAST_CLIENT_HPP___
#define ___MULTICAST_CLIENT_HPP___

#include <Common.h>
#include <SoaDataType.h>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <arpa/inet.h>

class MulticastClientListener {
public:
	virtual ~MulticastClientListener() {};
	virtual void onReceived(int client_id, const std::uint8_t *data, const int &len) = 0;
};

class MulticastClient
{
private:
	int mClientId;
	int mSocket;
	std::string mMulticastIpStr;
	int mPort;
	bool mIsLocal;
	bool mIsPortCreated;
	std::atomic<bool> mIsThreadCreated;
	std::atomic<bool> mIsThreadRunning;
	sockaddr_in mMulticastAddr;
	ip_mreq mIpMReq;
	std::thread mThread;

	std::mutex mMutexForListenerList;
	std::list<std::shared_ptr<MulticastClientListener>> mListenerList;

	bool createPort();
	void runThread();

public:
	MulticastClient(const std::string &multicast_ip, const int &port, const bool &is_local);
	~MulticastClient();
	bool IsPortCreated();
	bool IsThreadCreated();
	bool IsThreadRunning();
	int GetClientId();
	std::string GetMulticastIpString();
	bool RunClient();
	bool StopClient();
	bool RegisterListener(const std::shared_ptr<MulticastClientListener> &listener);
};

class MulticastClientManager
{
private:
	static MulticastClientManager *mInst;

	std::mutex mMutexForClientList;
	std::list<std::shared_ptr<MulticastClient>> mClientList;

	MulticastClientManager();
	~MulticastClientManager();

public:
	static MulticastClientManager *GetInstance();
	static void RemoveInstance();

	int CreateClient(const std::string &multicast_ip, const int &port, const bool &is_local = true);
	bool DestroyClient(int client_id);
	std::shared_ptr<MulticastClient> GetClient(const int &client_id);
};

#endif