#ifndef __DMS_DATATYPE_CONVERTER_H__
#define __DMS_DATATYPE_CONVERTER_H__

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soadmsdriverinside.h"
#include "eevp/control/impl_type_soadmsfatigue.h"
#include "eevp/control/impl_type_soadmsgazingdir.h"
#include "eevp/control/impl_type_soadmsonphone.h"
#include "eevp/control/impl_type_soadmssmoking.h"

// DriverInside
extern eevp::control::SoaDmsDriverInside Convert_DmsDriverInside_SOA2API(DmsDriverInside_e inside);
extern DmsDriverInside_e Convert_DmsDriverInside_API2SOA(eevp::control::SoaDmsDriverInside inside);

// Fatigue
extern eevp::control::SoaDmsFatigue Convert_DmsFatigue_SOA2API(DmsFatigue_e fatigue);
extern DmsFatigue_e Convert_DmsFatigue_API2SOA(eevp::control::SoaDmsFatigue fatigue);

// GazingDir
extern eevp::control::SoaDmsGazingDir Convert_DmsGazingDir_SOA2API(DmsGazingDir_e dir);
extern DmsGazingDir_e Convert_DmsGazingDir_API2SOA(eevp::control::SoaDmsGazingDir dir);

// OnPhone
extern eevp::control::SoaDmsOnPhone Convert_DmsOnPhone_SOA2API(DmsOnPhone_e phone);
extern DmsOnPhone_e Convert_DmsOnPhone_API2SOA(eevp::control::SoaDmsOnPhone phone);

// Smoke
extern eevp::control::SoaDmsSmoking Convert_DmsSmoke_SOA2API(DmsSmoking_e smoke);
extern DmsSmoking_e Convert_DmsSmoke_API2SOA(eevp::control::SoaDmsSmoking smoke);

#endif // __DMS_DATATYPE_CONVERTER_H__