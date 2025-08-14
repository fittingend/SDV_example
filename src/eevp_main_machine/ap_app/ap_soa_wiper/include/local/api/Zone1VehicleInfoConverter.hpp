#ifndef __ZONE1_VEHICLEINFO_CONVERTER_HPP__
#define __ZONE1_VEHICLEINFO_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>

// GearState_e
extern std::uint8_t Convert_GearState_SOA2Zone(GearState_e gear);
extern GearState_e Convert_GearState_Zone2SOA(std::uint8_t gear);

// IgnState_e
extern std::uint8_t Convert_IgnState_SOA2Zone(IgnState_e ign);
extern IgnState_e Convert_IgnState_Zone2SOA(std::uint8_t ign);

// IgnState_e
extern std::uint8_t Convert_AccState_SOA2Zone(AccState_e acc);
extern AccState_e Convert_AccState_Zone2SOA(std::uint8_t acc);

#endif // __ZONE1_VEHICLEINFO_CONVERTER_HPP__
