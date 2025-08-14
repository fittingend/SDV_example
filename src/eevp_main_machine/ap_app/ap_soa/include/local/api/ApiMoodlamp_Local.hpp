#ifndef __API_MOODLAMP_LOCAL_HPP__
#define __API_MOODLAMP_LOCAL_HPP__

#include <Common.h>
#include <SoaDataType.h>

void Api_Moodlamp_Method_RequestMlmSetRgbColor(int index);
void Api_Moodlamp_Method_RequestMlmSetMode(MlmMoodMode_e mode);
void Api_Moodlamp_Method_RequestMlmSetBrightness(int brightness);
void Api_Moodlamp_Field_SoaMlmStatus();

#endif