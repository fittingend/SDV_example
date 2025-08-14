#ifndef __ZONE5_DMS_CONVERTER_HPP__
#define __ZONE5_DMS_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>

// DeviceNormal
extern uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_Zone2SOA(uint8_t isNormal);

// DriverInside
extern uint8_t Convert_DriverInside_SOA2Zone(DmsDriverInside_e inside);
extern DmsDriverInside_e Convert_DriverInside_Zone2SOA(uint8_t inside);

// Fatigue
extern uint8_t Convert_Fatigue_SOA2Zone(DmsFatigue_e fatigue);
extern DmsFatigue_e Convert_Fatigue_Zone2SOA(uint8_t fatigue);

// GazingDir
extern uint8_t Convert_GazingDir_SOA2Zone(DmsGazingDir_e dir);
extern DmsGazingDir_e Convert_GazingDir_Zone2SOA(uint8_t dir);

// OnPhone
extern uint8_t Convert_OnPhone_SOA2Zone(DmsOnPhone_e phone);
extern DmsOnPhone_e Convert_OnPhone_Zone2SOA(uint8_t phone);

// Smoke
extern uint8_t Convert_Smoke_SOA2Zone(DmsSmoking_e smoke);
extern DmsSmoking_e Convert_Smoke_Zone2SOA(uint8_t smoke);

#endif // __ZONE5_DMS_CONVERTER_HPP__