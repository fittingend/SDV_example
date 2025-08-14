#include <Common.h>
#include <SoaDataType.h>
#include <MessageItem.hpp>
#include <VehicleContext.hpp>
#include <MulticastClientManager.hpp>
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

static std::atomic<int> s_mNextClientId{1};

bool MulticastClient::createPort()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
	{
		LOG_ERROR() << "socket(AF_INET, SOCK_DGRAM, 0) failed:\n";
		return false;
	}

	// Timeout
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 70 * 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
	{
		LOG_ERROR() << "Failed to set receive timeout\n";
		close(sock);
		return false;
	}

	// Socket Reuse
	int opt = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
	{
		LOG_ERROR() << "Failed to set socket reuseable\n";
		close(sock);
		return false;
	}

	// bind socket
	this->mMulticastAddr.sin_family = AF_INET;
	this->mMulticastAddr.sin_port = htons(this->mPort);
	this->mMulticastAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (sockaddr*)&(this->mMulticastAddr), sizeof(this->mMulticastAddr)) < 0)
	{
		LOG_ERROR() << "Failed to bind socket.\n";
		close(sock);
		return false;
	}

	// Multicast Option
	inet_pton(AF_INET, this->mMulticastIpStr.c_str(), &(this->mIpMReq.imr_multiaddr));
	if (this->mIsLocal)
	{
		inet_pton(AF_INET, "127.0.0.1", &(this->mIpMReq.imr_interface));
	}

	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &(this->mIpMReq), sizeof(this->mIpMReq)) < 0)
	{
		LOG_ERROR() << "Failed to setsockopt for multicast.\n";
		close(sock);
		return false;
	}

	this->mSocket = sock;
	return true;
}

void MulticastClient::runThread()
{
	this->mIsThreadRunning = true;

	char buffer[1024];
	while (this->mIsThreadRunning)
	{
		int bytesRead = recv(this->mSocket, buffer, sizeof(buffer), 0);
		if (bytesRead < 0)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN) // recv timeout:
			{
				// LOG_DEBUG() << "recv(this->mSocket, ...) timeout occurred.\n";
				continue;
			}
			else
			{
				LOG_ERROR() << "Error reading from client\n";
			}
		}
		else if (bytesRead == 0)
		{
			LOG_ERROR() << "disconnected\n";
		}
		else // Received Data:
		{
			std::unique_lock<std::mutex> lock(this->mMutexForListenerList);
			for (auto listener : this->mListenerList)
			{
				listener->onReceived(this->mClientId, (const std::uint8_t *)buffer, bytesRead);
			}
		}
	}

	LOG_DEBUG() << "...Now receive thread finishing...\n";
}

MulticastClient::MulticastClient(const std::string &multicast_ip, const int &port, const bool &is_local)
{
	this->mSocket = -1;
	this->mMulticastIpStr = multicast_ip;
	this->mPort = port;
	this->mIsLocal = is_local;
	this->mClientId = s_mNextClientId++;
	memset(&(this->mMulticastAddr), 0, sizeof(this->mMulticastAddr));
	memset(&(this->mIpMReq), 0, sizeof(this->mIpMReq));

	this->mIsPortCreated = this->createPort();
	this->mIsThreadCreated = false;
	this->mIsThreadRunning = false;
}

MulticastClient::~MulticastClient()
{
	{
		std::unique_lock<std::mutex> lock(this->mMutexForListenerList);
		this->mListenerList.clear();
	}

	if (this->mIsThreadRunning)
	{
		this->mIsThreadRunning = false;
	}

	if (this->mIsThreadCreated)
	{
		this->mThread.join();
		this->mIsThreadCreated = false;
	}

	if (this->mIsPortCreated)
	{
		setsockopt(this->mSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, &(this->mIpMReq), sizeof(this->mIpMReq));
		close(this->mSocket);
		this->mIsPortCreated = false;
	}
}

bool MulticastClient::IsPortCreated()
{
	return this->mIsPortCreated;
}

bool MulticastClient::IsThreadCreated()
{
	return this->mIsThreadCreated;
}

bool MulticastClient::IsThreadRunning()
{
	return this->mIsThreadRunning;
}

int MulticastClient::GetClientId()
{
	return (this->mIsPortCreated) ? this->mClientId : -1;
}

std::string MulticastClient::GetMulticastIpString()
{
	return (this->mIsPortCreated) ? this->mMulticastIpStr : "";
}

bool MulticastClient::RunClient()
{
	if (!this->mIsThreadCreated)
	{
		this->mThread = std::thread(&MulticastClient::runThread, this);
		this->mIsThreadCreated = true;
		return true;
	}

	return false;
}

bool MulticastClient::StopClient()
{
	if (this->mIsThreadCreated)
	{
		this->mIsThreadRunning = false;
		this->mThread.join();
		this->mIsThreadCreated = false;
		return true;
	}

	if (this->mIsPortCreated)
	{
		setsockopt(this->mSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, &(this->mIpMReq), sizeof(this->mIpMReq));
		close(this->mSocket);
		this->mIsPortCreated = false;
	}

	return false;
}

bool MulticastClient::RegisterListener(const std::shared_ptr<MulticastClientListener> &listener)
{
	std::unique_lock<std::mutex> lock(this->mMutexForListenerList);
	for (auto _listener : this->mListenerList)
	{
		if (listener.get() == _listener.get())
		{
			LOG_ERROR() << "Get the same listener added. exit.\n";
			return false;
		}
	}

	this->mListenerList.push_back(listener);
	return true;
}


MulticastClientManager *MulticastClientManager::mInst = nullptr;

MulticastClientManager *MulticastClientManager::GetInstance()
{
	if (MulticastClientManager::mInst == nullptr)
	{
		MulticastClientManager::mInst = new MulticastClientManager();
	}

	return MulticastClientManager::mInst;
}

void MulticastClientManager::RemoveInstance()
{
	if (MulticastClientManager::mInst != nullptr)
	{
		delete MulticastClientManager::mInst;
		MulticastClientManager::mInst = nullptr;
	}
}

MulticastClientManager::MulticastClientManager()
{

}

MulticastClientManager::~MulticastClientManager()
{
	std::unique_lock<std::mutex> lock(this->mMutexForClientList);
	this->mClientList.clear();
}

int MulticastClientManager::CreateClient(const std::string &multicast_ip, const int &port, const bool &is_local)
{
	std::unique_lock<std::mutex> lock(this->mMutexForClientList);

	LOG_DEBUG() << "(+) multicast_ip=[" << multicast_ip << ":" << port << "]\n";

	for (auto client : this->mClientList)
	{
		std::string client_ip = client->GetMulticastIpString();
		if (client_ip == multicast_ip)
		{
			LOG_ERROR() << "Error: same ip exists...\n";
			LOG_DEBUG() << "(-) ret=-1\n";
			return -1;
		}
	}

	std::shared_ptr<MulticastClient> client = std::make_shared<MulticastClient>(multicast_ip, port, is_local);
	if (!client->IsPortCreated())
	{
		LOG_ERROR() << "Error: new client [" << multicast_ip << ":" << port << "] not working...\n";
		LOG_DEBUG() << "(-) ret=-1\n";
		return -1;
	}

	this->mClientList.push_back(client);
	LOG_DEBUG() << "(-) ret(client_id)=" << client->GetClientId() << "\n";
	return client->GetClientId();
}

bool MulticastClientManager::DestroyClient(int client_id)
{
	std::unique_lock<std::mutex> lock(this->mMutexForClientList);

	for (auto it = this->mClientList.begin(); it != this->mClientList.end(); it++)
	{
		if ((*it)->GetClientId() == client_id)
		{
			this->mClientList.erase(it);
			return true;
		}
	}

	return false;
}

std::shared_ptr<MulticastClient> MulticastClientManager::GetClient(const int &client_id)
{
	std::unique_lock<std::mutex> lock(this->mMutexForClientList);

	for (auto client : this->mClientList)
	{
		if (client->GetClientId() == client_id)
		{
			return client;
		}
	}

	return nullptr;
}