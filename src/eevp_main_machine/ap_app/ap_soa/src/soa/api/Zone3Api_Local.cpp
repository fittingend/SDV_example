#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <string>
#include <Zone3Api_Local.hpp>
#if defined(ZONE_COMM_UDP)
#include <UdpServer.hpp>
#endif
#include <iostream>
#include <Log.hpp>

#define BUFFER_SIZE 100

void Zone3_RCtn_PowerOn(PowerState_e power)
{
#if !defined(ZONE_COMM_UDP)
#else
    UdpServer *inst = UdpServer::GetInstance();

    std::vector<UdpPosValue_t> diffs;
    udpPosValue_t item(75);

    switch (power)
    {
    case ePowerState_On:
        item.mValue = 0x01;
        break;
    case ePowerState_Off:
        item.mValue = 0x00;
        break;
    default:
        return;
    }

    diffs.push_back(item);
    inst->SendDataDiffToZone(2, diffs);

    LOG_DEBUG() << "[Zone3_RCtn_PowerOn] power(" << power << ")\n";
#endif
}

void Zone3_RCtn_MoveCurtainMotor(RCtnSwitch_e btn)
{
#if !defined(ZONE_COMM_UDP)
#else
    UdpServer *inst = UdpServer::GetInstance();

    std::vector<UdpPosValue_t> diffs;
    udpPosValue_t item(77);

    switch (btn)
    {
    case eRCtnSwitch_Off:
        item.mValue = 0x00;
        break;
    case eRCtnSwitch_OpenOn:
        item.mValue = 0x02;
        break;
    case eRCtnSwitch_CloseOn:
        item.mValue = 0x01;
        break;
    default:
        return;
    }

    diffs.push_back(item);
    inst->SendDataDiffToZone(2, diffs);

    LOG_DEBUG() << "[Zone3_RCtn_MoveCurtainMotor] RCtnSwitch_e(" << btn << ")\n";
#endif
}