#ifndef ___MULTICAST_SERVER_HPP___
#define ___MULTICAST_SERVER_HPP___

#include <Common.h>
#include <SoaDataType.h>
#include <mutex>
#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <arpa/inet.h>
#include <net/if.h>

class MulticastServer
{
private:
	std::mutex mMutexForServer;

	int mServerId;
	int mSocket;
	std::string mMulticastIpStr;
	int mPort;
	bool mIsLocal;
	bool mIsRunning;
	sockaddr_in mMulticastAddr;
	struct in_addr mLocalInterface;

public:
	MulticastServer(const std::string &multicast_ip, const int &port, const bool &is_local);
	~MulticastServer();
	bool IsRunning();
	int GetServerId();
	std::string GetMulticastIpString();
	bool SendData(const uint8_t *data, const int &data_size);

};

class MulticastServerManager
{
private:
	static MulticastServerManager *mInst;

	std::mutex mMutexForServerList;
	std::list<std::shared_ptr<MulticastServer>> mServerList;

	MulticastServerManager();
	~MulticastServerManager();

public:
	static MulticastServerManager *GetInstance();
	static void RemoveInstance();

	int CreateServer(const std::string &multicast_ip, const int &port, const bool &is_local = true);
	bool DestroyServer(const int &server_id);
	std::shared_ptr<MulticastServer> GetServer(const int &server_id);
};

#endif