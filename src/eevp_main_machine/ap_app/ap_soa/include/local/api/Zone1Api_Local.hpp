#ifndef __ZONE_1_API_LOCAL_H__
#define __ZONE_1_API_LOCAL_H__

#include <Common.h>
#include <SoaDataType.h>

void Zone1_MoodLamp_PowerOn(PowerState_e power);
void Zone1_MoodLamp_SetColorIndex(int colorIndex);
void Zone1_MoodLamp_SetBrightness(int brightness);
void Zone1_MoodLamp_SetColorMode(MlmMoodMode_e eMoodMode);

#endif