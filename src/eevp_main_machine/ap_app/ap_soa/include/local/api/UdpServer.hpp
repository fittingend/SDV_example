#ifndef __UDP_SERVER_HPP__
#define __UDP_SERVER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <thread>
#include <atomic>
#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <arpa/inet.h>
#include <UdpParser.hpp>
#if defined(UDP_SEND_REPEAT)
#include <MessageQueue.hpp>
#endif

typedef struct udpPosValue_t
{
    int mPosition;
    unsigned char mValue;

    udpPosValue_t()
    {
        mPosition = -1;
        mValue = 0;
    }

    udpPosValue_t(int pos)
    {
        mPosition = pos;
        mValue = 0;
    }

    udpPosValue_t(int pos, unsigned char val)
    {
        mPosition = pos;
        mValue = val;
    }

} UdpPosValue_t;

class UdpServer
{
private:
    static UdpServer *mInst;
#if defined(ZONE_COMM_UDP_MULTICAST)
    int mSenderSocket;
    int mReceiverSocket;
#else
    int mServerSocket;
#endif
    bool mThreadCreated;
#if defined(ZONE_COMM_UDP_MULTICAST)
    struct sockaddr_in mSenderAddr;
#else
    struct sockaddr_in mZoneAddress[5];
    bool mZoneAddrReceived[5];
#endif
    unsigned char *mSendingBuffer[5];

    std::atomic<bool> mThreadRunning;
    std::atomic<bool> mServerCreated;
    std::thread mThread;
    UdpParser *mParser;
    std::atomic<bool> mFlagSentPrint;
    std::atomic<bool> mFlagReceivedPrint;

#if defined(UDP_SEND_REPEAT)
    MessageQueue<int> mSendQueue;
    std::thread mSendThread;
    std::atomic<bool> mSendThreadRunning;
    std::mutex mMutexForSending[5];
#endif
    UdpServer();
    ~UdpServer();

    void RunServer(void);
    int CreateServer(void);
    int ReceiveData(void);
    void MaskReceivedData(sockaddr_in addr, unsigned char *buffer, int len);
    unsigned char *GetSendBuffer(int zoneId);
    int SendDataToZone(int zoneId, unsigned char *buffer, int size);
#if defined(UDP_SEND_REPEAT)
    void RunSendThread(void);
#endif
    void PrintBinaryToSstream(std::stringstream *ss, unsigned char *data, int len);

public:
    static UdpServer *GetInstance();
    void Start(void);
    void Stop(void);
    int SendDataDiffToZone(int zoneId, std::vector<UdpPosValue_t> diffs);
    void SetSentDataPrint(bool flag);
    void SetReceivedDataPrint(bool flag);
};

#endif