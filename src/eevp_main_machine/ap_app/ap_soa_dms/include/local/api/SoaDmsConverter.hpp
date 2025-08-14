#ifndef __SOA_DMS_CONVERTER_HPP__
#define __SOA_DMS_CONVERTER_HPP__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soadmsdriverinside.h"
#include "eevp/control/impl_type_soadmsfatigue.h"
#include "eevp/control/impl_type_soadmsgazingdir.h"
#include "eevp/control/impl_type_soadmsonphone.h"
#include "eevp/control/impl_type_soadmssmoking.h"

// DeviceNormal
extern eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal);
extern DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal);

// DriverInside
extern eevp::control::SoaDmsDriverInside Convert_DriverInside_SOA2API(DmsDriverInside_e inside);
extern DmsDriverInside_e Convert_DriverInside_API2SOA(eevp::control::SoaDmsDriverInside inside);

// Fatigue
extern eevp::control::SoaDmsFatigue Convert_Fatigue_SOA2API(DmsFatigue_e fatigue);
extern DmsFatigue_e Convert_Fatigue_API2SOA(eevp::control::SoaDmsFatigue fatigue);

// GazingDir
extern eevp::control::SoaDmsGazingDir Convert_GazingDir_SOA2API(DmsGazingDir_e dir);
extern DmsGazingDir_e Convert_GazingDir_API2SOA(eevp::control::SoaDmsGazingDir dir);

// OnPhone
extern eevp::control::SoaDmsOnPhone Convert_OnPhone_SOA2API(DmsOnPhone_e phone);
extern DmsOnPhone_e Convert_OnPhone_API2SOA(eevp::control::SoaDmsOnPhone phone);

// Smoke
extern eevp::control::SoaDmsSmoking Convert_Smoke_SOA2API(DmsSmoking_e smoke);
extern DmsSmoking_e Convert_Smoke_API2SOA(eevp::control::SoaDmsSmoking smoke);

#endif // __SOA_DMS_CONVERTER_HPP__