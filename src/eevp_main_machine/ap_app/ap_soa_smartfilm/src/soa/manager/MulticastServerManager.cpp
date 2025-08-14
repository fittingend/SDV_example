#include <Common.h>
#include <SoaDataType.h>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <MulticastServerManager.hpp>
#include <arpa/inet.h>
#include <cstdint>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <atomic>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>

#include <Log.hpp>
#include <Debug_DataType2String.hpp>

static std::atomic<int> s_mNextServerId{1};

MulticastServer::MulticastServer(const std::string &multicast_ip, const int &port, const bool &is_local)
{
	this->mIsRunning = false;
	this->mMulticastIpStr = multicast_ip;
	this->mPort = port;
	this->mIsLocal = is_local;
	this->mServerId = s_mNextServerId++;
	this->mSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->mSocket >= 0)
	{
		memset(&(this->mMulticastAddr), 0, sizeof(this->mMulticastAddr));
		memset(&(this->mLocalInterface), 0, sizeof(this->mLocalInterface));

		this->mMulticastAddr.sin_family = AF_INET;
		this->mMulticastAddr.sin_port = htons(this->mPort);
		inet_pton(AF_INET, this->mMulticastIpStr.c_str(), &(this->mMulticastAddr.sin_addr));

		if (this->mIsLocal)
		{
			inet_pton(AF_INET, "127.0.0.1", &(this->mLocalInterface));
			setsockopt(this->mSocket, IPPROTO_IP, IP_MULTICAST_IF, &(this->mLocalInterface), sizeof(this->mLocalInterface));
		}

		unsigned char loop = 1;
		setsockopt(this->mSocket, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));

		this->mIsRunning = true;
	}
}

MulticastServer::~MulticastServer()
{
	std::unique_lock<std::mutex> lock(this->mMutexForServer);
	if (this->mIsRunning)
	{
		close(this->mSocket);
		this->mIsRunning = false;
	}
}

bool MulticastServer::IsRunning()
{
	std::unique_lock<std::mutex> lock(this->mMutexForServer);
	return this->mIsRunning;
}

int MulticastServer::GetServerId()
{
	std::unique_lock<std::mutex> lock(this->mMutexForServer);
	return (this->mIsRunning) ? this->mServerId : -1;
}

std::string MulticastServer::GetMulticastIpString()
{
	std::unique_lock<std::mutex> lock(this->mMutexForServer);
	return (this->mIsRunning) ? this->mMulticastIpStr : "";
}

bool MulticastServer::SendData(const uint8_t *data, const int & data_size)
{
	std::unique_lock<std::mutex> lock(this->mMutexForServer);
	int ret = -1;
	if (this->mIsRunning)
	{
		ret = sendto(this->mSocket, (char *)data, data_size, 0, (sockaddr *)&(this->mMulticastAddr), sizeof(this->mMulticastAddr));
	}
	return (ret >= 0);
}

MulticastServerManager *MulticastServerManager::mInst = nullptr;

MulticastServerManager *MulticastServerManager::GetInstance()
{
	if (MulticastServerManager::mInst == nullptr)
	{
		MulticastServerManager::mInst = new MulticastServerManager();
	}

	return MulticastServerManager::mInst;
}

void MulticastServerManager::RemoveInstance()
{
	if (MulticastServerManager::mInst != nullptr)
	{
		delete MulticastServerManager::mInst;
		MulticastServerManager::mInst = nullptr;
	}
}

MulticastServerManager::MulticastServerManager()
{

}

MulticastServerManager::~MulticastServerManager()
{
	std::unique_lock<std::mutex> lock(this->mMutexForServerList);
	this->mServerList.clear();
}

int MulticastServerManager::CreateServer(const std::string &multicast_ip, const int &port, const bool &is_local)
{
	std::unique_lock<std::mutex> lock(this->mMutexForServerList);

	LOG_DEBUG() << "(+) multicast_ip=[" << multicast_ip << ":" << port << "]\n";

	for (auto server : this->mServerList)
	{
		std::string server_ip = server->GetMulticastIpString();
		if (server_ip == multicast_ip)
		{
			LOG_ERROR() << "Error: same ip exists...\n";
			LOG_DEBUG() << "(-) ret=-1\n";
			return -1;
		}
	}

	std::shared_ptr<MulticastServer> server = std::make_shared<MulticastServer>(multicast_ip, port, is_local);
	if (!server->IsRunning())
	{
		LOG_ERROR() << "Error: new server [" << multicast_ip << ":" << port << "] not working...\n";
		LOG_DEBUG() << "(-) ret=-1\n";
		return -1;
	}

	this->mServerList.push_back(server);
	LOG_DEBUG() << "(-) ret(server_id)=" << server->GetServerId() << "\n";
	return server->GetServerId();
}

bool MulticastServerManager::DestroyServer(const int &server_id)
{
	std::unique_lock<std::mutex> lock(this->mMutexForServerList);

	for (auto it = this->mServerList.begin(); it != this->mServerList.end(); it++)
	{
		if ((*it)->GetServerId() == server_id)
		{
			this->mServerList.erase(it);
			return true;
		}
	}

	return false;
}

std::shared_ptr<MulticastServer> MulticastServerManager::GetServer(const int &server_id)
{
	std::unique_lock<std::mutex> lock(this->mMutexForServerList);

	for (auto server : this->mServerList)
	{
		if (server->GetServerId() == server_id)
		{
			return server;
		}
	}

	return nullptr;
}