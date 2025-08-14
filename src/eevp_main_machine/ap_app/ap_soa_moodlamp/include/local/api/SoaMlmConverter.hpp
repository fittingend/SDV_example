#ifndef __SOA_MOODLAMP_CONVERTER_HPP__
#define __SOA_MOODLAMP_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soamlmmoodmode.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

// MoodMode
extern eevp::control::SoaMlmMoodMode Convert_MoodMode_SOA2API(MlmMoodMode_e mode);
extern MlmMoodMode_e Convert_MoodMode_API2SOA(eevp::control::SoaMlmMoodMode mode);

#endif // __SOA_MOODLAMP_CONVERTER_HPP__

