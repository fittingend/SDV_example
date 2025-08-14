#include <Common.h>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <CmdMgr.hpp>
#include <TelnetServer.hpp>
#include <CmdLists.hpp>
#include <Log.hpp>

#define TELNET_PORT 31463

TelnetServer *TelnetServer::mInst = nullptr;

TelnetServer *TelnetServer::GetInstance()
{
	if (TelnetServer::mInst == nullptr)
	{
		TelnetServer::mInst = new TelnetServer();
	}

	return TelnetServer::mInst;
}

void TelnetServer::DestroyInstance()
{
	if (TelnetServer::mInst != nullptr)
	{
		delete TelnetServer::mInst;
		TelnetServer::mInst = nullptr;
	}
}

TelnetServer::TelnetServer()
{
	CmdMgr *cmdMgr = CmdMgr::GetInstance();
	this->mClientSocket = -1;
	this->mServerSocket = -1;
	this->mThreadCreated = false;
	this->mThreadRunning = false;
	this->mTelnetReady = false;
	this->mDisconnectClient = false;
	memset(this->mInputCmd, 0, 1024);
	this->mInputPointer = 0;

	cmdMgr->RegisterMsgSender(this);
	//RegisterUdpParserCommands(this->mCmdMgr);
	RegisterEcuApi();
	RegisterCmdContext();
}

TelnetServer::~TelnetServer()
{

}

void TelnetServer::Start(void)
{
	this->mThread = std::thread(&TelnetServer::RunServer, this);
	this->mThreadCreated = true;
}

void TelnetServer::Stop(void)
{
	this->mThreadRunning = false;
	this->mThread.join();
	this->mThreadCreated = false;

	CmdMgr::DestroyInstance();
}

void TelnetServer::RunServer()
{
	int retVal;

	this->mThreadRunning = true;
	while (this->mThreadRunning)
	{
		retVal = this->CreateServer();
		if (retVal != 0)
		{
			LOG_ERROR() << "create server failed:\n";
			this->ExecuteErrorState();
			continue;
		}

		retVal = this->ConnectClient();
		if (retVal != 0)
		{
			LOG_ERROR() << "connection failed:\n";
			this->ExecuteErrorState();
			continue;
		}

		retVal = this->CommunicateWithClient();
		if (retVal != 0)
		{
			LOG_ERROR() << "connection failed:\n";
		}

		this->ExecuteErrorState();
	}
}

int TelnetServer::CreateServer(void)
{
	// 소켓 생성
	this->mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if ( this->mServerSocket == -1) {
		LOG_ERROR() << "Error creating socket\n";
		this->mServerSocket = -1;
		return -1;
	}

	// 주소 구조체 설정
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(TELNET_CMD_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Timeout
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 70 * 1000;
	if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
	{
		LOG_ERROR() << "Failed to set receive timeout\n";
		return -1;
	}

	// Socket Reuse
	int opt = 1;
	if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
	{
		LOG_ERROR() << "Failed to set socket reuseable\n";
		return -1;
	}

	// 소켓에 주소 할당
	if (bind(this->mServerSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
		LOG_ERROR() << "Error binding socket\n";
		close(this->mServerSocket);
		this->mServerSocket = -1;
		return -1;
	}

	// 연결 요청 대기
	if (listen(this->mServerSocket, 5) == -1) {
		LOG_ERROR() << "Error listening on socket\n";
		close(this->mServerSocket);
		this->mServerSocket = -1;
		return -1;
	}

	LOG_DEBUG() << "Server started, waiting for connections...\n";
	return 0;
}

int TelnetServer::ConnectClient(void)
{
	// 연결 수락
	while (this->mThreadRunning)
	{
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);
		this->mClientSocket = accept(this->mServerSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
		if (this->mClientSocket  == -1)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
			{
				// Timeout: Loop if it is just timeout
				continue;
			}

			LOG_ERROR() << "Error accepting connection\n";
			return -1;
		}

		LOG_DEBUG() << "Client connected\n";
		return 0;
	}

	LOG_ERROR() << "Telnet Server Stopped\n";
	return -1;
}

int TelnetServer::CommunicateWithClient(void)
{
	// 클라이언트와의 통신
	LOG_DEBUG() << "ReadyTelnet\n";
	std::thread readyThread = std::thread(&TelnetServer::ReadyTelnet, this);
	LOG_DEBUG() << "ReadyTelnet End\n";

	this->mDisconnectClient = false;

	char buffer[1024];
	while (this->mThreadRunning && !this->mDisconnectClient) {
		int bytesRead = recv(this->mClientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead < 0)
		{
			if (errno == EWOULDBLOCK || errno == EAGAIN)
			{
				// Timeout: Loop if it is just timeout
				continue;
			}

			LOG_ERROR() << "Error reading from client\n";
			break;
		} else if (bytesRead == 0) {
			LOG_DEBUG() << "Client disconnected\n";
			break;
		} else {
			if (this->mTelnetReady)
			{
				this->ReadCommand(buffer, bytesRead);
			}
			else
			{
				buffer[bytesRead] = '\0';
				send(this->mClientSocket, buffer, strlen(buffer), 0);
			}
		}
	}

	readyThread.join();
	return 1;
}

int TelnetServer::ReadCommand(char buffer[], int length)
{
	char sendBuff[16];
	int i = 0;
	while (i < length)
	{
		if (buffer[i] == 0x08) // backspace
		{
			if (this->mInputPointer > 0)
			{
				this->mInputCmd[this->mInputPointer] = 0x00;
				this->mInputPointer--;
			}
			else
			{
				// Send Right: reset position
				sendBuff[0] = 0x1b;
				sendBuff[1] = 0x5b;
				sendBuff[2] = 0x43;
				send(this->mClientSocket, sendBuff, 3, 0);
			}

			i++;
		}
		else if ((buffer[i] == 0x1b) && (buffer[i + 1] == 0x5b) && (buffer[i + 2] == 0x41))   // UP
		{
			// Do nothing, except resetting the position
			sendBuff[0] = 0x1b;
			sendBuff[1] = 0x5b;
			sendBuff[2] = 0x42;
			send(this->mClientSocket, sendBuff, 3, 0);
			i += 3;
		}
		else if ((buffer[i] == 0x1b) && (buffer[i + 1] == 0x5b) && (buffer[i + 2] == 0x42))   // DOWN
		{
			// Do nothing, except resetting the position
			sendBuff[0] = 0x1b;
			sendBuff[1] = 0x5b;
			sendBuff[2] = 0x41;
			send(this->mClientSocket, sendBuff, 3, 0);
			i += 3;
		}
		else if ((buffer[i] == 0x1b) && (buffer[i + 1] == 0x5b) && (buffer[i + 2] == 0x43))   // RIGHT
		{
			// Do nothing, except resetting the position
			sendBuff[0] = 0x1b;
			sendBuff[1] = 0x5b;
			sendBuff[2] = 0x44;
			send(this->mClientSocket, sendBuff, 3, 0);
			i += 3;
		}
		else if ((buffer[i] == 0x1b) && (buffer[i + 1] == 0x5b) && (buffer[i + 2] == 0x44))   // LEFT
		{
			// Do nothing, except resetting the position
			sendBuff[0] = 0x1b;
			sendBuff[1] = 0x5b;
			sendBuff[2] = 0x43;
			send(this->mClientSocket, sendBuff, 3, 0);
			i += 3;
		}
		else if ((buffer[i] == 0x0d) || (buffer[i] == 0x0a)) // Enter
		{
			sprintf(sendBuff, "\r\n");
			send(this->mClientSocket, sendBuff, strlen(sendBuff), 0);

			if (this->mInputPointer > 0)
			{
				this->mInputCmd[this->mInputPointer] = '\0';
				CmdMgr::GetInstance()->ExecuteCommand(this->mInputCmd);

				// Reset Command
				memset(this->mInputCmd, 0, 1024);
				this->mInputPointer = 0;
			}

			while (((buffer[i] == 0x0d) || (buffer[i] == 0x0a)) && (i < length))
			{
				i++;
			}

			strcpy(sendBuff, CmdMgr::GetInstance()->GetPrompt().c_str());
			send(this->mClientSocket, sendBuff, strlen(sendBuff), 0);
		}
		else
		{
			this->mInputCmd[this->mInputPointer] = buffer[i];
			this->mInputPointer ++;
			i ++;
		}
	}

	return 0;
}

void TelnetServer::ReadyTelnet()
{
	LOG_DEBUG() << "(+)\n";
	usleep(100 * 1000);  // 100ms

	char buffer[1024];

	std::stringstream ss;
	ss << APP_NAME << " connected: \r\n" << CmdMgr::GetInstance()->GetPrompt();
	strcpy(buffer, ss.str().c_str());
	send(this->mClientSocket, buffer, strlen(buffer), 0);

	this->mMutexForUnsent.lock();
	for (std::string string : this->mUnsentStringBuffer)
	{
		strcpy(buffer, string.c_str());
		send(this->mClientSocket, buffer, strlen(buffer), 0);
	}
	this->mUnsentStringBuffer.clear();
	this->mMutexForUnsent.unlock();

	this->mTelnetReady = true;
	LOG_DEBUG() << "(-)\n";
}

int TelnetServer::CleanResources(void)
{
	int retVal;

	this->mTelnetReady = false;
	if (this->mClientSocket >= 0)
	{
		retVal = close(this->mClientSocket);
		if (retVal != 0)
		{
			LOG_ERROR() << "close(this->mClientSocket) err:\n";
			return retVal;
		}
		this->mClientSocket = -1;
	}

	if (this->mServerSocket >= 0)
	{
		retVal = close(this->mServerSocket);
		if (retVal != 0)
		{
			LOG_ERROR() << "close(this->mServerSocket) err:\n";
			return retVal;
		}
		this->mServerSocket = -1;
	}

	return 0;
}

void TelnetServer::ExecuteErrorState(void)
{
	int count, retVal;
	for (count = 0; count < 5; count++)
	{
		retVal = this->CleanResources();
		usleep(50 * 1000);  // 150ms
		if (retVal == 0)
		{
			LOG_DEBUG() << "Clean succeeded.\n";
			return;
		}

		LOG_ERROR() << "Close Error with count " << count << ", sleep 200ms\n";
		usleep(150 * 1000);  // 150ms
	}

	LOG_ERROR() << "Close failed\n";
}

void TelnetServer::SendMessage(std::string message)
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);

	if (this->mTelnetReady)
	{
		const char *msg = message.c_str();
		send(this->mClientSocket, msg, strlen(msg), 0);
	}
	else
	{
		this->mMutexForUnsent.lock();
		this->mUnsentStringBuffer.push_back(message);
		if (this->mUnsentStringBuffer.size() > TELNET_LOG_RESERVE_LINE)
		{
			this->mUnsentStringBuffer.pop_front();
		}
		this->mMutexForUnsent.unlock();
	}
}

void TelnetServer::Disconnect(void)
{
	std::unique_lock<std::recursive_mutex> lock(this->mMutexForApi);
	this->mDisconnectClient = true;
}

