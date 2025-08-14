#ifndef __SOA_HVAC_CONVERTER_HPP__
#define __SOA_HVAC_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soahvacairsource.h"
#include "eevp/control/impl_type_soahvacblowdir.h"
#include "eevp/control/impl_type_soahvacstatus.h"
#include "eevp/control/impl_type_soahvactemps.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

// AirSource
extern eevp::control::SoaHvacAirSource Convert_AirSource_SOA2API(HvacAirSource_e airSrc);
extern HvacAirSource_e Convert_AirSource_API2SOA(eevp::control::SoaHvacAirSource airSrc);

// BlowDir
extern eevp::control::SoaHvacBlowDir Convert_BlowDir_SOA2API(HvacBlowDir_e blowDir);
extern HvacBlowDir_e Convert_BlowDir_API2SOA(eevp::control::SoaHvacBlowDir blowDir);

// Status
extern void Convert_HvacStatus_SOA2API(const HvacStatus_t &src, eevp::control::SoaHvacStatus &dst);
extern void Convert_HvacStatus_API2SOA(const eevp::control::SoaHvacStatus &src, HvacStatus_t &dst);
extern bool CompareAndUpdate_HvacStatus_API(const eevp::control::SoaHvacStatus &src, eevp::control::SoaHvacStatus &dst, bool update = true);

// Temps
extern void Convert_HvacTemps_SOA2API(const HvacTemps_t &src, eevp::control::SoaHvacTemps &dst);
extern void Convert_HvacTemps_API2SOA(const eevp::control::SoaHvacTemps &src, HvacTemps_t &dst);
extern bool CompareAndUpdate_HvacTemps_API(const eevp::control::SoaHvacTemps &src, eevp::control::SoaHvacTemps &dst, bool update = true);
#endif // __SOA_HVAC_CONVERTER_HPP__