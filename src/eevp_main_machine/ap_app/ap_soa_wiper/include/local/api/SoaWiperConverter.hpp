#ifndef __SOA_WIPER_CONVERTER_HPP__
#define __SOA_WIPER_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soawipermode.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

// Wiper Mode
extern eevp::control::SoaWiperMode Convert_WiperMode_SOA2API(WiperOpMode_e mode);
extern WiperOpMode_e Convert_WiperMode_API2SOA(eevp::control::SoaWiperMode mode);

#endif // __SOA_WIPER_CONVERTER_HPP__
