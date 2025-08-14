#include <Common.h>
#include <SoaDataType.h>
#include <string.h>
#include <string>
#include <Zone1Api_Local.hpp>
#if defined(ZONE_COMM_UDP)
#include <UdpServer.hpp>
#endif
#include <iostream>
#include <Log.hpp>

void Zone1_MoodLamp_PowerOn(PowerState_e power)
{
#if !defined(ZONE_COMM_UDP)
#else
    UdpServer *inst = UdpServer::GetInstance();

    std::vector<UdpPosValue_t> diffs;
    udpPosValue_t item(25);

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
    inst->SendDataDiffToZone(0, diffs);

    LOG_DEBUG() << "[Zone1_MoodLamp_PowerOn] power(" << power << ")\n";
#endif
}

void Zone1_MoodLamp_SetColorIndex(int colorIndex)
{
#if !defined(ZONE_COMM_UDP)
#else
    if (colorIndex >= 0x00 and colorIndex <= 0x4a)
    {
        UdpServer *inst = UdpServer::GetInstance();

        std::vector<UdpPosValue_t> diffs;
        udpPosValue_t item(27, (unsigned char)colorIndex);
        diffs.push_back(item);
        inst->SendDataDiffToZone(0, diffs);
        LOG_DEBUG() << "[Zone1_MoodLamp_SetColorIndex] index(" << colorIndex << ")\n";
    }
#endif
}

void Zone1_MoodLamp_SetBrightness(int brightness)
{
#if !defined(ZONE_COMM_UDP)
#else
    if (brightness >= 0x00 and brightness <= 0xff)
    {
        UdpServer *inst = UdpServer::GetInstance();

        std::vector<UdpPosValue_t> diffs;
        udpPosValue_t item(28, (unsigned char)brightness);
        diffs.push_back(item);
        inst->SendDataDiffToZone(0, diffs);
        LOG_DEBUG() << "[Zone1_MoodLamp_SetBrightness] brightness(" << brightness << ")\n";
    }
#endif
}

void Zone1_MoodLamp_SetColorMode(MlmMoodMode_e eMoodMode)
{
#if !defined(ZONE_COMM_UDP)
#else
    UdpServer *inst = UdpServer::GetInstance();

    std::vector<UdpPosValue_t> diffs;
    udpPosValue_t item(29);

    switch (eMoodMode)
    {
    case eMlmMoodMode_CareMood:
        item.mValue = 0x00;
        break;
    case eMlmMoodMode_ComfortMood:
        item.mValue = 0x01;
        break;
    case eMlmMoodMode_DelightMood:
        item.mValue = 0x02;
        break;
    case eMlmMoodMode_MoodTherapy:
        item.mValue = 0x03;
        break;
    case eMlmMoodMode_VitalityMood:
        item.mValue = 0x04;
        break;
    default:
        return;
    }

    diffs.push_back(item);
    inst->SendDataDiffToZone(0, diffs);

    LOG_DEBUG() << "[Zone1_MoodLamp_SetColorMode] MoodMode(" << eMoodMode << ")\n";
#endif
}
