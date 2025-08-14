#ifndef __SOA_ROA_CONVERTER_HPP__
#define __SOA_ROA_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soaroarunningstate.h"
#include "eevp/control/impl_type_soaroamode.h"
#include "eevp/control/impl_type_soaroasensorerror.h"
#include "eevp/control/impl_type_soaroadetectstate.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

// RunningState
extern eevp::control::SoaRoaRunningState Convert_RunningState_SOA2API(RoaRunningState_e runningState);
extern RoaRunningState_e Convert_RunningState_API2SOA(eevp::control::SoaRoaRunningState runningState);

// ROA Mode
extern eevp::control::SoaRoaMode Convert_RoaMode_SOA2API(RoaMode_e mode);
extern RoaMode_e Convert_RoaMode_API2SOA(eevp::control::SoaRoaMode mode);

// Sensor Error
extern eevp::control::SoaRoaSensorError Convert_SensorError_SOA2API(RoaSensorError_e err);
extern RoaSensorError_e Convert_SensorError_API2SOA(eevp::control::SoaRoaSensorError err);

// Detect State
extern eevp::control::SoaRoaDetectState Convert_DetectState_SOA2API(int count);
extern int Convert_DetectState_API2SOA(eevp::control::SoaRoaDetectState count);

#endif // __SOA_ROA_CONVERTER_HPP__

