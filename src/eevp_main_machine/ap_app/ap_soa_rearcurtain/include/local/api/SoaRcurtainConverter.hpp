#ifndef __SOA_DMS_CONVERTER_HPP__
#define __SOA_DMS_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soaerrorstate.h"
#include "eevp/control/impl_type_soarctnmotordir.h"
#include "eevp/control/impl_type_soarctnstate.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(const DeviceNormal_e &isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(const eevp::control::SoaDeviceIsNormal &isNormal);

// ErrorState
extern eevp::control::SoaErrorState Convert_ErrorState_SOA2API(const ErrorState_e &error);
extern ErrorState_e Convert_DriverInside_API2SOA(const eevp::control::SoaErrorState &error);

// Motor Direction
extern eevp::control::SoaRctnMotorDir Convert_MotorDir_SOA2API(const RCtnMotorDir_e &dir);
extern RCtnMotorDir_e Convert_MotorDir_API2SOA(const eevp::control::SoaRctnMotorDir &dir);

// Motor Position
extern uint8_t Convert_MotorPos_SOA2API(const RCtnPos_e &pos);
extern RCtnPos_e Convert_MotorPos_API2SOA(const uint8_t &pos);

// RearCurtain State
extern eevp::control::SoaRctnState Convert_RctnState_SOA2API(const RCtnState_e &state);
extern RCtnState_e Convert_RctnState_API2SOA(const eevp::control::SoaRctnState &state);

// MotorPos <-> State
extern RCtnState_e Convert_MotorPos2RctnState(const RCtnPos_e &pos);

#endif // __SOA_DMS_CONVERTER_HPP__

