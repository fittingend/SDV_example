#ifndef __API_MOODLAMP_LOCAL_HPP__
#define __API_MOODLAMP_LOCAL_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void SOA_MoodLamp_Init(void);
extern void SOA_MoodLamp_Term(void);
extern void SOA_MoodLamp_SetColorIndex(int index);
extern void SOA_MoodLamp_SetMode(MlmMoodMode_e mode);
extern void SOA_MoodLamp_SetBrightness(int brightness);
extern void SOA_MoodLamp_UpdateStatus();

#endif