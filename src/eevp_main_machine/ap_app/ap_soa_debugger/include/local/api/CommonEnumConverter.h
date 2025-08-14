#ifndef __COMMON_ENUM_CONVERTER_H__
#define __COMMON_ENUM_CONVERTER_H__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

#endif // __COMMON_ENUM_CONVERTER_H__