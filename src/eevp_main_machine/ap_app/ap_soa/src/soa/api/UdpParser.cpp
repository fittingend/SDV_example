#include <Common.h>
#include <SoaDataType.h>
#include <VehicleContext.hpp>
#include <RearCurtainManager.hpp>
#include <MoodLampManager.hpp>

#include <atomic>
#include <functional>
#include <UdpParser.hpp>

UdpParser *UdpParser::mInst = nullptr;

template<typename T>
struct ParsedInfo {
    bool isChanged;
    T curr;
    T prev;
#if defined(VEHICLE_CONTEXT_USE_ATOMIC)
    std::atomic<T> *dest;
#else
    T *dest;
#endif
    std::function<void(int, int)> func;

    ParsedInfo() {
        isChanged = false;
        dest = nullptr;
        func = nullptr;
    }
};

inline static int get16Bit(unsigned char *data)
{
    return (((int)data[0]) << 8) | (int)data[1];
}

inline static ParsedInfo<int> parseVehicleSpeed(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<int> info;
    int val1 = (int)data[0];
    int val2 = (int)data[1];
    if (data[0] != 0xAA || data[1] != 0xAA)
    {
        info.prev = context->mVehicleSpeed;
        info.dest = &(context->mVehicleSpeed);
        info.func = [](int curr, int prev) {
            RearCurtainManager *mgr = RearCurtainManager::GetInstance();
            mgr->OnEventVehiclSpeed(curr, prev);
        };

        int value = get16Bit(data);
        info.curr = (value >= 0 && value <= 510) ? value : info.prev;
        info.isChanged = (info.curr != info.prev);
    }

    return info;
}

inline static ParsedInfo<GearState_e> parseGearState(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<GearState_e> info;
    if (data[2] != 0xAA)
    {
        info.prev = context->mGearState;
        info.dest = &(context->mGearState);
        info.func = [](int curr, int prev) {
            RearCurtainManager *mgr = RearCurtainManager::GetInstance();
            mgr->OnEventGearState((GearState_e)curr, (GearState_e)prev);
        };

        switch (data[2])
        {
        case 0x00:
            info.curr = eGearState_P;
            break;
        case 0x01:
            info.curr = eGearState_D;
            break;
        case 0x02:
            info.curr = eGearState_N;
            break;
        case 0x03:
            info.curr = eGearState_R;
            break;
        case 0x05:
            info.curr = eGearState_Fault;
            break;
        default:
            info.curr = info.prev;
            break;
        }

        info.isChanged = (info.curr != info.prev);
    }

    return info;
}

inline static ParsedInfo<PowerState_e> parseRearCurtainPower(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<PowerState_e> info;
    if (data[53] != 0xAA)
    {
        info.prev = context->mRCPower;
        info.dest = &(context->mRCPower);
        info.func = [](int curr, int prev) {
            RearCurtainManager *mgr = RearCurtainManager::GetInstance();
            mgr->OnEventPower((PowerState_e)curr, (PowerState_e)prev);
        };

        switch (data[53])
        {
        case 0x00:
            info.curr = ePowerState_Off;
            break;
        case 0x01:
            info.curr = ePowerState_On;
            break;
        case 0x03:
            info.curr = ePowerState_Error;
            break;
        default:
            info.curr = info.prev;
            break;
        }

        info.isChanged = (info.curr != info.prev);
    }

    return info;
}


inline static ParsedInfo<RCtnSwitch_e> parseRearCurtainSwitch(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<RCtnSwitch_e> info;
    if (data[11] != 0xAA)
    {
        info.prev = context->mRCSwitch;
        info.dest = &(context->mRCSwitch);
        info.func = [](int curr, int prev) {
            RearCurtainManager *mgr = RearCurtainManager::GetInstance();
            mgr->OnEventRcButton((RCtnSwitch_e)curr, (RCtnSwitch_e)prev);
        };

        switch (data[11])
        {
        case 0x00:
            info.curr = eRCtnSwitch_Off;
            break;
        case 0x01:
            info.curr = eRCtnSwitch_CloseOn;    // UP: Close
            break;
        case 0x02:
            info.curr = eRCtnSwitch_OpenOn;     // Down: Open
            break;
        default:
            info.curr = info.prev;
            break;
        }

        info.isChanged = (info.curr != info.prev);
    }

    return info;
}

inline static ParsedInfo<RCtnState_e> parseRearCurtainState(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<RCtnState_e> info;
    if (data[55] != 0xAA || data[56] != 0xAA || data[57] != 0xAA)
    {
        info.prev = context->mRCState;
        info.dest = &(context->mRCState);
        info.func = [](int curr, int prev) {
            RearCurtainManager *mgr = RearCurtainManager::GetInstance();
            mgr->OnEventRcState((RCtnState_e)curr, (RCtnState_e)prev);
        };

        int motorMove = (int)data[55];
        if (motorMove == 0xAA)
        {
            switch (context->mRCState)
            {
            case eRCtnState_Opening:
                motorMove = 0x02;   // Down:Opening
                break;
            case eRCtnState_Closing:
                motorMove = 0x01;   // UP:Closing
                break;
            default:
                motorMove = 0x00;
                break;
            }
        }

        int motorPos = (int)data[56];
        if (motorPos == 0xAA)
        {
            switch (context->mRCState)
            {
            case eRCtnState_FullyClosed:
                motorPos = 0x01;    // Up Limit -> Fully Closed
                break;
            case eRCtnState_FullyOpened:
                motorPos = 0x02;    // Down Limit -> Fully Opened
                break;
            default:
                motorMove = 0x00;
                break;
            }
        }

        int motorErr = (int)data[57];
        if (motorErr == 0xAA)
        {
            switch (context->mRCState)
            {
            case eRCtnState_Error:
                motorErr = 0x01;
                break;
            default:
                motorErr = 0x00;
                break;
            }
        }

        if (motorErr == 0x01)
        {
            info.curr = eRCtnState_Error;
        }
        else if (motorPos == 0x01)
        {
            info.curr = eRCtnState_FullyClosed;
        }
        else if (motorPos == 0x02)
        {
            info.curr = eRCtnState_FullyOpened;
        }
        else if (motorMove == 0x02)
        {
            info.curr = eRCtnState_Opening;
        }
        else if (motorMove == 0x01)
        {
            info.curr = eRCtnState_Closing;
        }
        else if (info.prev == eRCtnState_Opening || info.prev == eRCtnState_Closing)
        {
            info.curr = eRCtnState_PartlyOpened;
        }
        else
        {
            info.curr = info.prev;
        }

        info.isChanged = (info.curr != info.prev);
    }

    return info;
}

inline static ParsedInfo<PowerState_e> parseMoodlampPower(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<PowerState_e> info;
    if (data[22] != 0xAA)
    {
        info.prev = context->mMlmPower;
        info.dest = &(context->mMlmPower);
        info.func = [](int curr, int prev) {

        };

        switch (data[22])
        {
        case 0x00:
            info.curr = ePowerState_Off;
            break;
        case 0x01:
            info.curr = ePowerState_On;
            break;
        case 0x03:
            info.curr = ePowerState_Error;
            break;
        default:
            info.curr = info.prev;
            break;
        }

        info.isChanged = (info.curr != info.prev);
    }

    return info;
}

inline static ParsedInfo<int> parseMoodlampColorIndex(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<int> info;
    if (data[14] != 0xAA)
    {
        info.prev = context->mMlmColorIndex;
        info.dest = &(context->mMlmColorIndex);
        info.func = [](int curr, int prev) {

        };

        int value = (int)data[14];
        if (value >= 0 && value <= 0x4A)
        {
            info.curr = value;
            info.isChanged = (info.curr != info.prev);
#if defined(BLOCK_MOODLAMP_STATUS_FROM_ZONE1)
            info.isChanged = false;
#endif
        }
    }

    return info;
}

inline static ParsedInfo<int> parseMoodlampBrightness(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<int> info;
    if (data[15] != 0xAA)
    {
        info.prev = context->mMlmColorIndex;
        info.dest = &(context->mMlmColorIndex);
        info.func = [](int curr, int prev) {

        };

        int value = (int)data[15];
        if (value >= 0 && value <= 0x7F)
        {
            info.curr = value;
            info.isChanged = (info.curr != info.prev);
#if defined(BLOCK_MOODLAMP_STATUS_FROM_ZONE1)
            info.isChanged = false;
#endif
       }
    }

    return info;
}

inline static ParsedInfo<MlmMoodMode_e> parseMoodlampMoodMode(VehicleContext *context, unsigned char *data)
{
    ParsedInfo<MlmMoodMode_e> info;
    if (data[16] != 0xAA)
    {
        info.prev = context->mMlmMoodMode;
        info.dest = &(context->mMlmMoodMode);
        info.func = [](int curr, int prev) {

        };

        switch ((int)data[16])
        {
        case 0x00:
            info.curr = eMlmMoodMode_CareMood;
            break;
        case 0x01:
            info.curr = eMlmMoodMode_ComfortMood;
            break;
        case 0x02:
            info.curr = eMlmMoodMode_DelightMood;
            break;
        case 0x03:
            info.curr = eMlmMoodMode_MoodTherapy;
            break;
        case 0x04:
            info.curr = eMlmMoodMode_VitalityMood;
            break;
        default:
            info.curr = info.prev;
            break;
        }

        info.isChanged = (info.curr != info.prev);
#if defined(BLOCK_MOODLAMP_STATUS_FROM_ZONE1)
        info.isChanged = false;
#endif
    }

    return info;
}

template <typename T>
inline static void UpdateAndSendToActionManager (ParsedInfo<T> info)
{
    if (info.isChanged)
    {
        *(info.dest) = info.curr;
        if (info.func != nullptr)
        {
            info.func((int)info.curr, (int)info.prev);
        }
    }
}

UdpParser *UdpParser::GetInstance()
{
    if (UdpParser::mInst == nullptr)
    {
        UdpParser::mInst = new UdpParser();
    }

    return UdpParser::mInst;
}

UdpParser::UdpParser()
{
    this->mContext = VehicleContext::GetInstance();
}

UdpParser::~UdpParser()
{

}

int UdpParser::ParseData(unsigned char *data)
{
    // vehicle speed
    ParsedInfo<int> vehicleSpeed = parseVehicleSpeed(this->mContext, data);

    // gear speed
    ParsedInfo<GearState_e> gearState = parseGearState(this->mContext, data);

    // Rear Curtain Power
    ParsedInfo<PowerState_e> rcPower = parseRearCurtainPower(this->mContext, data);

    // Rear Curtain Switch
    ParsedInfo<RCtnSwitch_e> rcSwitch = parseRearCurtainSwitch(this->mContext, data);

    // Rear Curtain State
    ParsedInfo<RCtnState_e> rcState = parseRearCurtainState(this->mContext, data);

    // Mood Lamp Power
    ParsedInfo<PowerState_e> mlmPower = parseMoodlampPower(this->mContext, data);

    // Mood Lamp Color Index
    ParsedInfo<int> mlmColorIndex = parseMoodlampColorIndex(this->mContext, data);

    // Mood Lamp Brightness
    ParsedInfo<int> mlmBrightness = parseMoodlampBrightness(this->mContext, data);

    // Mood Lamp Mood Mode
    ParsedInfo<MlmMoodMode_e> mlmMoodMode = parseMoodlampMoodMode(this->mContext, data);

    // Execute all Items...
    UpdateAndSendToActionManager(vehicleSpeed);
    UpdateAndSendToActionManager(gearState);
    UpdateAndSendToActionManager(rcPower);
    UpdateAndSendToActionManager(rcSwitch);
    UpdateAndSendToActionManager(rcState);
    UpdateAndSendToActionManager(mlmPower);
    UpdateAndSendToActionManager(mlmColorIndex);
    UpdateAndSendToActionManager(mlmBrightness);
    UpdateAndSendToActionManager(mlmMoodMode);

    return 0;
}