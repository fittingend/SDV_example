#ifndef __ZONE4_WIPER_CONVERTER_HPP__
#define __ZONE4_WIPER_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>

// DeviceNormal
extern std::uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_Zone2SOA(std::uint8_t isNormal);

// DriverInside
extern std::uint8_t Convert_WiperMode_SOA2Zone(WiperOpMode_e wiperOp);
extern WiperOpMode_e Convert_WiperMode_Zone2SOA(std::uint8_t wiperOp);

#endif // __ZONE4_WIPER_CONVERTER_HPP__

