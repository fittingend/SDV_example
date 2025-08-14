#ifndef __ZONE5_HVAC_CONVERTER_HPP__
#define __ZONE5_HVAC_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>
#include <zone5/hvac/control/impl_type_ecuz5hvacstatus.h>
#include <zone5/hvac/control/impl_type_ecuz5hvactemps.h>

// DeviceNormal
extern uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_Zone2SOA(uint8_t isNormal);

// AirSource
extern uint8_t Convert_AirSource_SOA2Zone(HvacAirSource_e airSrc);
extern HvacAirSource_e Convert_AirSource_Zone2SOA(uint8_t airSrc);

// BlowDir
extern uint8_t Convert_BlowDir_SOA2Zone(HvacBlowDir_e blowDir);
extern HvacBlowDir_e Convert_BlowDir_Zone2SOA(uint8_t blowDir);

// Status
extern void Convert_HvacStatus_SOA2Zone(const HvacStatus_t &src, zone5::hvac::control::EcuZ5HvacStatus &dst);
extern void Convert_HvacStatus_Zone2SOA(const zone5::hvac::control::EcuZ5HvacStatus &src, HvacStatus_t &dst);

// Temps
extern void Convert_HvacTemps_SOA2Zone(const HvacTemps_t &src, zone5::hvac::control::EcuZ5HvacTemps &dst);
extern void Convert_HvacTemps_Zone2SOA(const zone5::hvac::control::EcuZ5HvacTemps &src, HvacTemps_t &dst);

#endif // __ZONE5_HVAC_CONVERTER_HPP__