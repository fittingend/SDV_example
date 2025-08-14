#include <Common.h>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <cerrno>
#include <UdpParser.hpp>
#include <UdpServer.hpp>
#include <Log.hpp>

#define RECEIVE_BUFFER_SIZE 1024

typedef struct {
    int startPos;
    int endPos;
} BinZoneArea_t;

static bool s_ZoneRunning[] = { true, false, true, false, false };
static BinZoneArea_t s_ZoneArea[] = {
    { 0, 24 },
    { -1, -1 },
    { 50, 74 },
    { -1, -1 },
    { -1, -1 },
};

UdpServer *UdpServer::mInst = nullptr;

UdpServer *UdpServer::GetInstance()
{
    if (UdpServer::mInst == nullptr)
    {
        UdpServer::mInst = new UdpServer();
        UdpServer::mInst->Start();
    }

    return UdpServer::mInst;
}

UdpServer::UdpServer()
{
#if defined(ZONE_COMM_UDP_MULTICAST)
    this->mSenderSocket = -1;
    this->mReceiverSocket = -1;
#else
    this->mServerSocket = -1;
#endif
    this->mThreadCreated = false;
    this->mThreadRunning = false;
    this->mServerCreated = false;
    this->mFlagSentPrint = false;
    this->mFlagReceivedPrint = false;
#if defined(UDP_SEND_REPEAT)
    this->mSendThreadRunning = false;
#endif

    // Address for Zone1 ~ 3
#if !defined(ZONE_COMM_UDP_MULTICAST)
    char *zone_addr[] = { (char *)ZONE1_IP, (char *)ZONE2_IP, (char *)ZONE3_IP, (char *)ZONE4_IP, (char *)ZONE5_IP };
    int zone_port[] = { ZONE1_PORT, ZONE2_PORT, ZONE3_PORT, ZONE4_PORT, ZONE5_PORT };
    for (int i = 0; i < 5; i++)
    {
        struct sockaddr_in *addr = &(this->mZoneAddress[i]);
        memset(addr, 0, sizeof(struct sockaddr_in));
        addr->sin_family = AF_INET;
        addr->sin_port = htons(zone_port[i]);
        inet_pton(AF_INET, zone_addr[i], &(addr->sin_addr));
        this->mZoneAddrReceived[i] = false;
        this->mSendingBuffer[i] = (unsigned char *)malloc(100);
        memset(this->mSendingBuffer[i], 0xAA, 100);
    }

    this->mSendingBuffer[0][25] = 0;
    this->mSendingBuffer[0][27] = 0;
    this->mSendingBuffer[0][28] = 0;

    this->mSendingBuffer[2][75] = 0;
    this->mSendingBuffer[2][77] = 0;
#endif
}

UdpServer::~UdpServer()
{
    for (int i = 0; i < 5; i++)
    {
        free(this->mSendingBuffer[i]);
    }
}

void UdpServer::Start(void)
{
    this->mThread = std::thread(&UdpServer::RunServer, this);
    this->mThreadCreated = true;
}

void UdpServer::Stop(void)
{
    this->mThreadRunning = false;
    this->mThread.join();

#if defined(UDP_SEND_REPEAT)
    if (this->mSendThreadRunning)
    {
        this->mSendThread.join();
    }
#endif

    this->mThreadCreated = false;
}

void UdpServer::RunServer(void)
{
    int retVal;

    this->mThreadRunning = true;
    this->mParser = UdpParser::GetInstance();

    while (this->mThreadRunning)
    {
        retVal = this->CreateServer();
        if (retVal == 0)
        {
            break;
        }

        LOG_ERROR() << "[UdpServer::RunServer] create server failed:\n";
        // this->ExecuteErrorState();
    }

#if defined(UDP_SEND_REPEAT)
    if (this->mThreadRunning)
    {
        this->mSendThread = std::thread(&UdpServer::RunSendThread, this);
        this->mSendThreadRunning = true;
    }
#endif

    this->mServerCreated = true;
    while (this->mThreadRunning)
    {
        retVal = this->ReceiveData();
        if (retVal != 0)
        {
            LOG_ERROR() << "[UdpServer::RunServer] receive failed:\n";
        }
    }
}

int UdpServer::CreateServer(void)
{
#if defined(ZONE_COMM_UDP_MULTICAST)
    // Sender Socket Create
    this->mSenderSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->mSenderSocket == -1) {
        LOG_ERROR() << "[UdpServer::CreateServer] Error creating sender socket\n";
        goto ERROR;
    }

    // Sender Address Create
    memset(&(this->mSenderAddr), 0, sizeof(this->mSenderAddr));
    this->mSenderAddr.sin_family = AF_INET;
    this->mSenderAddr.sin_addr.s_addr = inet_addr(ZONE_UDP_MULTICAST_IP);
    this->mSenderAddr.sin_port = htons(ZONE_UDO_MULTICAST_PORT);

    // Receiver Socket Create
    this->mReceiverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->mReceiverSocket == -1) {
        LOG_ERROR() << "[UdpServer::CreateServer] Error creating receiver socket\n";
        goto ERROR;
    }

    // Socket Reuse
    int opt = 1;
    if (setsockopt(this->mReceiverSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
    {
        LOG_ERROR() << "[UdpServer::CreateServer] Failed to set socket reuseable\n";
        return -1;
    }

    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(ZONE_UDO_MULTICAST_PORT);
    if (bind(this->mReceiverSocket, (struct sockaddr*)&localAddr, sizeof(localAddr)) == -1) {
        LOG_ERROR() << "[UdpServer::CreateServer] Error binding receiver socket\n";
        goto ERROR;
    }

    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr(ZONE_UDP_MULTICAST_IP);
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(this->mReceiverSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&multicastRequest, sizeof(multicastRequest)) < 0) {
        LOG_ERROR() << "[UdpServer::CreateServer] Error in receiver Socket setsockopt\n";
    }
#else

    this->mServerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->mServerSocket == -1) {
        LOG_ERROR() << "[TelnetServer::CreateServer] Error creating socket\n";
        return -1;
    }

    // 주소 구조체 설정
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(UDP_PORT); // UDP Port
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Socket Reuse
    int opt = 1;
    if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) < 0)
    {
        LOG_ERROR() << "[UdpServer::CreateServer] Failed to set socket reuseable\n";
        return -1;
    }

    // 소켓에 주소 할당
    if (bind(this->mServerSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        LOG_ERROR() << "[UdpServer::CreateServer] Error binding socket\n";
        goto ERROR;
    }

    // Timeout
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 499000;
    if (setsockopt(this->mServerSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) < 0)
    {
        LOG_ERROR() << "[UdpServer::CreateServer] Failed to set receive timeout\n";
        return -1;
    }
#endif

    LOG_DEBUG() << "[UdpServer::CreateServer] Server started, waiting for connections...\n";
    return 0;

ERROR:
#if defined(ZONE_COMM_UDP_MULTICAST)
    if (this->mSenderSocket >= 0)
    {
        close(this->mSenderSocket);
        this->mSenderSocket = -1;
    }

    if (this->mReceiverSocket >= 0)
    {
        close(this->mReceiverSocket);
        this->mReceiverSocket = -1;
    }
#else
    if (this->mServerSocket >= 0)
    {
        close(this->mServerSocket);
        this->mServerSocket = -1;
    }
#endif
    return -1;
}


int UdpServer::ReceiveData(void)
{
    int bytesReceived;
    unsigned char buffer[RECEIVE_BUFFER_SIZE];
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

#if defined(ZONE_COMM_UDP_MULTICAST)
    bytesReceived = recvfrom(this->mReceiverSocket, buffer, RECEIVE_BUFFER_SIZE, 0, NULL, 0);
    if (bytesReceived < 0)
    {
         // Timeout:
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            LOG_ERROR() << "[UdpServer::ReceiveData] Timeout occurred.\n";
            return 0;
        }
        else
        {
            LOG_ERROR() << "[UdpServer::ReceiveData] recvfrom() error\n";
            return -1;
        }
    }
#else
    bytesReceived = recvfrom(this->mServerSocket, buffer, RECEIVE_BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (bytesReceived < 0)
    {
        // Timeout:
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            LOG_ERROR() << "[UdpServer::ReceiveData] Timeout occurred.\n";
            return 0;
        }
        else
        {
            LOG_ERROR() << "[UdpServer::ReceiveData] recvfrom() error\n";
            return -1;
        }
    }

    if (this->mFlagReceivedPrint)
    {
        std::stringstream ss;
        ss << "Received from " << std::string(inet_ntoa(clientAddr.sin_addr)) << ":\n";
        if (bytesReceived > 0)
        {
            this->PrintBinaryToSstream(&ss, buffer, bytesReceived);
        }
        else
        {
            ss << "bytesRead = 0\n";
        }

        LOG_DEBUG() << ss.str().c_str();
    }

#if defined(CHANGE_ZONEIP_BY_UDP_PACKETS)
    if (!this->mZoneAddrReceived[0] && clientAddrLen > 0)
    {
        for (int i = 0; i < 25; i++)
        {
            if (buffer[i] != 0xAA)
            {
                std::string address_str = std::string(inet_ntoa(clientAddr.sin_addr));
                memcpy(&(this->mZoneAddress[0]), &clientAddr, clientAddrLen);
                this->mZoneAddrReceived[0] = true;
                LOG_DEBUG() << "[UdpServer::ReceiveData] Zone1 Address [" << address_str << "] found.\n";
                break;
            }
        }
    }

    if (!this->mZoneAddrReceived[2] && clientAddrLen > 0)
    {
        for (int i = 50; i < 75; i++)
        {
            if (buffer[i] != 0xAA)
            {
                std::string address_str = std::string(inet_ntoa(clientAddr.sin_addr));
                memcpy(&(this->mZoneAddress[2]), &clientAddr, clientAddrLen);
                this->mZoneAddrReceived[2] = true;
                LOG_DEBUG() << "[UdpServer::ReceiveData] Zone3 Address [" << address_str << "] found.\n";
                break;
            }
        }
    }
#endif

    //this->MaskReceivedData(clientAddr, buffer, bytesReceived);
#endif

    this->mParser->ParseData(buffer);
    return 0;
}

void UdpServer::MaskReceivedData(sockaddr_in addr, unsigned char *buffer, int len)
{
    for (int i = 0; i < 5; i++)
    {
        if (s_ZoneRunning[i])
        {
            BinZoneArea_t *zoneArea = &(s_ZoneArea[i]);
            if (memcmp(&(this->mZoneAddress[i].sin_addr), &(addr.sin_addr), sizeof(addr.sin_addr)) == 0)
            {
                LOG_DEBUG() << "[UdpServer::MaskReceivedData] Data from zone [" << (i+1) << "]....\n";
                for (int j = 0; j < len; j++)
                {
                    if (j < zoneArea->startPos || j > zoneArea->endPos)
                    {
                        buffer[j] = 0xaa;
                    }
                }

                return;
            }
        }
    }
}

int UdpServer::SendDataToZone(int zoneId, unsigned char *buffer, int size)
{
#if defined(ZONE_COMM_UDP_MULTICAST)
    int result = sendto(this->mSenderSocket, buffer, size, 0, (struct sockaddr *)&(this->mSenderAddr), sizeof(struct sockaddr_in));
    if (result < 0)
    {
        LOG_ERROR() << "[UdpServer::SendDataToZone] Failed to send data... \n";
        return -1;
    }
#elif !defined(UDP_SEND_REPEAT)
    if (zoneId >= 0 && zoneId < 5)
    {
        struct sockaddr_in *addr = &(this->mZoneAddress[zoneId]);
        int result = sendto(this->mServerSocket, buffer, size, 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));
        if (result < 0)
        {
            LOG_ERROR() << "[UdpServer::SendDataToZone] Failed to send data... \n";
            return -1;
        }
    }
#else
    if (zoneId >= 0 && zoneId < 5)
    {
        this->mSendQueue.Push(zoneId);
    }
#endif

    return 0;
}

unsigned char *UdpServer::GetSendBuffer(int zoneId)
{
    if (zoneId >= 0 && zoneId < 5)
    {
#if !defined(UDP_SEND_REPEAT)
        memset(this->mSendingBuffer[zoneId], 0xaa, 100);
#endif
        return this->mSendingBuffer[zoneId];
    }

    return nullptr;
}

int UdpServer::SendDataDiffToZone(int zoneId, std::vector<UdpPosValue_t> diffs)
{
#if defined(UDP_SEND_REPEAT)
    std::unique_lock<std::mutex> lock(this->mMutexForSending[zoneId]);
#endif
    unsigned char *buffer = this->GetSendBuffer(zoneId);
    if (buffer == nullptr)
    {
        return -1;
    }

    for (UdpPosValue_t item : diffs)
    {
        if (item.mPosition >= 0 && item.mPosition < 100)
        {
            buffer[item.mPosition] = item.mValue;
        }
    }

    return this->SendDataToZone(zoneId, buffer, 100);
}



#if defined(UDP_SEND_REPEAT)
void UdpServer::RunSendThread(void)
{
    while (this->mThreadRunning)
    {
        int value = 0;
        this->mSendQueue.PopTimeout(&value, 499);
        for (int i = 0; i < 5; i++)
        {
            if (s_ZoneRunning[i])
            {
                std::unique_lock<std::mutex> lock(this->mMutexForSending[i]);
                struct sockaddr_in *addr = &(this->mZoneAddress[i]);
                int result = sendto(this->mServerSocket, this->mSendingBuffer[i], 100, 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in));

                if (result < 0)
                {
                    LOG_ERROR() << "[UdpServer::RunSendThread] Failed to send data to zone " << (i + 1) << "... \n";
                }
                else
                {
                    if (this->mFlagSentPrint)
                    {
                        std::stringstream ss;
                        ss << "Sent to " << std::string(inet_ntoa(addr->sin_addr)) << ":\n";
                        this->PrintBinaryToSstream(&ss, this->mSendingBuffer[i], 100);
                        LOG_DEBUG() << ss.str().c_str();
                    }
                }
            }
        }
    }
}
#endif

void UdpServer::PrintBinaryToSstream(std::stringstream *ss, unsigned char *data, int len)
{
    bool newLine = false;
    for (int i = 0; i < len; i++)
    {
        newLine = false;
        *ss << "[";
        *ss << std::setfill('0') << std::setw(2) << std::hex << (int)data[i];
        *ss << "]";

        if ((i % 25) == 24)
        {
            *ss << "\n";
            newLine = true;
        }
    }

    if (!newLine)
    {
        *ss << "\n";
    }
}

void UdpServer::SetSentDataPrint(bool flag)
{
    this->mFlagSentPrint = flag;
}

void UdpServer::SetReceivedDataPrint(bool flag)
{
    this->mFlagReceivedPrint = flag;
}