#ifndef __ZONE5_RCURTAIN_CONVERTER_HPP__
#define __ZONE5_RCURTAIN_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>

// DeviceNormal
extern uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_Zone2SOA(uint8_t isNormal);

// Motor Dir
extern uint8_t Convert_MotorDir_SOA2Zone(RCtnMotorDir_e button);
extern RCtnMotorDir_e Convert_MotorDir_Zone2SOA(uint8_t button);

// Position
extern uint8_t Convert_Position_SOA2Zone(RCtnPos_e pos);
extern RCtnPos_e Convert_Position_Zone2SOA(uint8_t pos);

#endif // __ZONE5_RCURTAIN_CONVERTER_HPP__

