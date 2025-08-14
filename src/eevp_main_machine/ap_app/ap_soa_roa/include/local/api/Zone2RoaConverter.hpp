#ifndef __ZONE2_ROA_CONVERTER_HPP__
#define __ZONE2_ROA_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>

// DeviceNormal
extern std::uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_Zone2SOA(std::uint8_t isNormal);

// Sensor Error
extern std::uint8_t Convert_SensorError_SOA2Zone(RoaSensorError_e err);
extern RoaSensorError_e Convert_SensorError_Zone2SOA(std::uint8_t err);

// DetectInfo2Count
extern int Convert_DetectInfo2Count_Zone2SOA(std::uint8_t detInfo);


#endif // __ZONE2_ROA_CONVERTER_HPP__

