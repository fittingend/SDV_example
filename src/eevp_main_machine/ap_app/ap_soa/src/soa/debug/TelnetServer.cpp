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

#define TELNET_PORT 31457

TelnetServer::TelnetServer()
{
    this->mCmdMgr = new CmdMgr(this);
    this->mClientSocket = -1;
    this->mServerSocket = -1;
    this->mThreadCreated = false;
    this->mThreadRunning = false;
    this->mTelnetReady = false;
    memset(this->mInputCmd, 0, 1024);
    this->mInputPointer = 0;

    RegisterUdpParserCommands(this->mCmdMgr);
    RegisterVehicleContextCommands(this->mCmdMgr);
    RegisterMoodLampCommands(this->mCmdMgr);
    RegisterRearCurtainCommands(this->mCmdMgr);
    RegisterUdpServerCommands(this->mCmdMgr);
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
            LOG_ERROR() << "[TelnetServer::RunServer] create server failed:\n";
            this->ExecuteErrorState();
            continue;
        }

        retVal = this->ConnectClient();
        if (retVal != 0)
        {
            LOG_ERROR() << "[TelnetServer::RunServer] connection failed:\n";
            this->ExecuteErrorState();
            continue;
        }

        retVal = this->CommunicateWithClient();
        if (retVal != 0)
        {
            LOG_ERROR() << "[TelnetServer::RunServer] connection failed:\n";
        }

        this->ExecuteErrorState();
    }
}

int TelnetServer::CreateServer(void)
{
    // 소켓 생성
    this->mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( this->mServerSocket == -1) {
        LOG_ERROR() << "[TelnetServer::CreateServer] Error creating socket\n";
        this->mServerSocket = -1;
        return -1;
    }

    // 주소 구조체 설정
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(TELNET_PORT); // 텔넷 포트
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Timeout
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
    {
        LOG_ERROR() << "[TelnetServer::CreateServer] Failed to set receive timeout\n";
        return -1;
    }

    // Socket Reuse
    int opt = 1;
    if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
    {
        LOG_ERROR() << "[TelnetServer::CreateServer] Failed to set socket reuseable\n";
        return -1;
    }

    // 소켓에 주소 할당
    if (bind(this->mServerSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        LOG_ERROR() << "[TelnetServer::CreateServer] Error binding socket\n";
        close(this->mServerSocket);
        this->mServerSocket = -1;
        return -1;
    }

    // 연결 요청 대기
    if (listen(this->mServerSocket, 5) == -1) {
        LOG_ERROR() << "[TelnetServer::CreateServer] Error listening on socket\n";
        close(this->mServerSocket);
        this->mServerSocket = -1;
        return -1;
    }

    LOG_DEBUG() << "[TelnetServer::CreateServer] Server started, waiting for connections...\n";
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

            LOG_ERROR() << "[TelnetServer::ConnectClient] Error accepting connection\n";
            return -1;
        }

        LOG_DEBUG() << "[TelnetServer::ConnectClient] Client connected\n";
        return 0;
    }

    LOG_ERROR() << "[TelnetServer::ConnectClient] Telnet Server Stopped\n";
    return -1;
}

int TelnetServer::CommunicateWithClient(void)
{
    // 클라이언트와의 통신
    LOG_DEBUG() << "[TelnetServer::CommunicateWithClient] ReadyTelnet\n";
    std::thread readyThread = std::thread(&TelnetServer::ReadyTelnet, this);
    LOG_DEBUG() << "[TelnetServer::CommunicateWithClient] ReadyTelnet End\n";


    char buffer[1024];
    while (this->mThreadRunning) {
        int bytesRead = recv(this->mClientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead < 0)
        {
            if (errno == EWOULDBLOCK || errno == EAGAIN)
            {
                // Timeout: Loop if it is just timeout
                continue;
            }

            LOG_ERROR() << "[TelnetServer] Error reading from client\n";
            break;
        } else if (bytesRead == 0) {
            LOG_DEBUG() << "[TelnetServer] Client disconnected\n";
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
        else if ((buffer[i] == 0x0d) && (buffer[i + 1] == 0x00) || (buffer[i] == 0x0d) && (buffer[i + 1] == 0x0a)) // Enter
        {
            sprintf(sendBuff, "\r\n");
            send(this->mClientSocket, sendBuff, strlen(sendBuff), 0);

            if (this->mInputPointer > 0)
            {
                this->mInputCmd[this->mInputPointer] = '\0';
                this->mCmdMgr->ExecuteCommand(this->mInputCmd);

                // Reset Command
                memset(this->mInputCmd, 0, 1024);
                this->mInputPointer = 0;
            }

            i += 2;
            sprintf(sendBuff, "\r\n>> ");
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
    LOG_DEBUG() << "[TelnetServer::ReadyTelnet] (+)\n";
    usleep(100 * 1000);  // 100ms
    char buffer[1024];
    sprintf(buffer, "SOA connected: \r\n>> ");
    send(this->mClientSocket, buffer, strlen(buffer), 0);
    this->mTelnetReady = true;
    LOG_DEBUG() << "[TelnetServer::ReadyTelnet] (-)\n";
}

int TelnetServer::CleanResources(void)
{
    int retVal;

    if (this->mClientSocket >= 0)
    {
        retVal = close(this->mClientSocket);
        if (retVal != 0)
        {
            LOG_ERROR() << "[TelnetServer::CleanResources] close(this->mClientSocket) err:\n";
            return retVal;
        }
        this->mClientSocket = -1;
    }

    if (this->mServerSocket >= 0)
    {
        retVal = close(this->mServerSocket);
        if (retVal != 0)
        {
            LOG_ERROR() << "[TelnetServer::CleanResources] close(this->mServerSocket) err:\n";
            return retVal;
        }
        this->mServerSocket = -1;
    }

    this->mTelnetReady = false;
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
            LOG_DEBUG() << "[TelnetServer::ExecuteErrorState] Clean succeeded.\n";
            return;
        }

        LOG_ERROR() << "[TelnetServer::ExecuteErrorState] Close Error with count " << count << ", sleep 200ms\n";
        usleep(150 * 1000);  // 150ms
    }

    LOG_ERROR() << "[TelnetServer::ExecuteErrorState] Close failed\n";
}

void TelnetServer::SendMessage(std::string message)
{
    if (this->mClientSocket > 0)
    {
        const char *msg = message.c_str();
        send(this->mClientSocket, msg, strlen(msg), 0);
    }
}

