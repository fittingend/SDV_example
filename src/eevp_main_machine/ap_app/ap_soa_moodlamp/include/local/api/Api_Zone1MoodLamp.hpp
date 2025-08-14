#ifndef __ZONE_1_API_LOCAL_H__
#define __ZONE_1_API_LOCAL_H__

#include <Common.h>
#include <SoaDataType.h>

extern void Zone1_MoodLamp_Init(void);
extern void Zone1_MoodLamp_Term(void);
extern void Zone1_MoodLamp_PowerOn(PowerState_e power);
extern void Zone1_MoodLamp_SetColorIndex(int colorIndex);
extern void Zone1_MoodLamp_SetBrightness(int brightness);
extern void Zone1_MoodLamp_SetColorMode(MlmMoodMode_e eMoodMode);
extern void Zone1_MoodLamp_UpdateStatus(void);

#endif