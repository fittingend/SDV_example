#ifndef ___SOA_VEHICLEINFO_CONVERTER_HPP___
#define ___SOA_VEHICLEINFO_CONVERTER_HPP___

#include <Common.h>
#include <SoaDataType.h>
#include "eevp/control/impl_type_soagearstate.h"
#include "eevp/control/impl_type_soaignstate.h"
#include "eevp/control/impl_type_soaaccstate.h"

// GearState_e
extern eevp::control::SoaGearState Convert_GearState_SOA2API(GearState_e ign);
extern GearState_e Convert_GearState_API2SOA(eevp::control::SoaGearState ign);

// IgnState_e
extern eevp::control::SoaIgnState Convert_IgnState_SOA2API(IgnState_e ign);
extern IgnState_e Convert_IgnState_API2SOA(eevp::control::SoaIgnState ign);

// AccState_e
extern eevp::control::SoaAccState Convert_AccState_SOA2API(AccState_e acc);
extern AccState_e Convert_AccState_API2SOA(eevp::control::SoaAccState acc);


#endif // ___SOA_VEHICLEINFO_CONVERTER_HPP___