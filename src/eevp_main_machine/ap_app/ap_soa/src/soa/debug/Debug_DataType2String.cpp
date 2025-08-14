#include <Common.h>
#include <SoaDataType.h>
#include <string>
#include <Debug_DataType2String.hpp>

std::string getStringEnum_PowerState_e(PowerState_e val)
{
    switch (val)
    {
    case ePowerState_On:    return "ON";
    case ePowerState_Off:   return "OFF";
    case ePowerState_Error: return "Error";
    default: break;
    }

    return "UNKNOWN";
}

std::string getStringEnum_GearState_e(GearState_e val)
{
    switch (val)
    {
    case eGearState_P:      return "P";
    case eGearState_D:      return "D";
    case eGearState_N:      return "N";
    case eGearState_R:      return "R";
    case eGearState_Fault:  return "Fault";
    default:                break;
    }

    return "UNKNOWN";
}

std::string getStringEnum_RCtnSwitch_e(RCtnSwitch_e val)
{
    switch (val)
    {
    case eRCtnSwitch_Off:   return "OFF";
    case eRCtnSwitch_OpenOn:    return "OPEN_ON";
    case eRCtnSwitch_CloseOn:   return "CLOSE_ON";
    default:                    break;
    }

    return "UNKNOWN";

}

std::string getStringEnum_RCtnState_e(RCtnState_e val)
{
    switch (val)
    {
    case eRCtnState_FullyOpened:    return "FullyOpened";
    case eRCtnState_FullyClosed:    return "FullyClosed";
    case eRCtnState_PartlyOpened:   return "PartlyOpened";
    case eRCtnState_Opening:        return "Opening";
    case eRCtnState_Closing:        return "Closing";
    case eRCtnState_Unknown:        return "Unknow";
    case eRCtnState_Error:          return "Error";

    default:                        break;
    }

    return "UNKNOWN";
}

std::string getStringEnum_MlmMoodMode_e(MlmMoodMode_e val)
{
    switch (val)
    {
    case eMlmMoodMode_CareMood:         return "CareMood";
    case eMlmMoodMode_ComfortMood:      return "ComfortMood";
    case eMlmMoodMode_DelightMood:      return "DelightMood";
    case eMlmMoodMode_MoodTherapy:      return "MoodTherapy";
    case eMlmMoodMode_VitalityMood:     return "VitalityMood";

    default:                        break;
    }

    return "UNKNOWN";
}




