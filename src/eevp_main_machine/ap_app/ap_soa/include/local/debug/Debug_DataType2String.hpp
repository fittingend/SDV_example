#ifndef __DEBUG_DATATYPE_TO_STRING_H__
#define __DEBUG_DATATYPE_TO_STRING_H__

#include <Common.h>
#include <SoaDataType.h>
#include <string>

std::string getStringEnum_PowerState_e(PowerState_e val);
std::string getStringEnum_GearState_e(GearState_e val);
std::string getStringEnum_RCtnSwitch_e(RCtnSwitch_e val);
std::string getStringEnum_RCtnState_e(RCtnState_e val);
std::string getStringEnum_MlmMoodMode_e(MlmMoodMode_e val);

#endif