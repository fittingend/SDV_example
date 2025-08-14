#ifndef __SOA_DMS_CONVERTER_HPP__
#define __SOA_DMS_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soaseatheightdirection.h"
#include "eevp/control/impl_type_soaseatreclinedirection.h"
#include "eevp/control/impl_type_soaseatslidedirection.h"
#include "eevp/control/impl_type_soaseattiltdirection.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

// SoaSeatHeightDirection
extern eevp::control::SoaSeatHeightDirection Convert_SeatHeightDir_SOA2API(DriverSeatDir_e heightDir);
extern DriverSeatDir_e Convert_SeatHeightDir_API2SOA(eevp::control::SoaSeatHeightDirection heightDir);

// SoaSeatReclineDirection
extern eevp::control::SoaSeatReclineDirection Convert_SeatReclineDir_SOA2API(DriverSeatDir_e reclineDir);
extern DriverSeatDir_e Convert_SeatReclineDir_API2SOA(eevp::control::SoaSeatReclineDirection reclineDir);

// SoaSeatSlideDirection
extern eevp::control::SoaSeatSlideDirection Convert_SeatSlideDir_SOA2API(DriverSeatDir_e slideDir);
extern DriverSeatDir_e Convert_SeatSlideDir_API2SOA(eevp::control::SoaSeatSlideDirection slideDir);

// SoaSeatTiltDirection
extern eevp::control::SoaSeatTiltDirection Convert_SeatTiltDir_SOA2API(DriverSeatDir_e tiltDir);
extern DriverSeatDir_e Convert_SeatTiltDir_API2SOA(eevp::control::SoaSeatTiltDirection tiltDir);

#endif // __SOA_DMS_CONVERTER_HPP__