#ifndef __API_SOA_WIPER_HPP__
#define __API_SOA_WIPER_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void SOA_Wiper_Init(void);
extern void SOA_Wiper_Term(void);
extern void SOA_Wiper_ControlWiper(WiperOpMode_e mode);
extern void SOA_Wiper_SetAutoSpeed(bool isAutoSpeed);
extern void SOA_Wiper_UpdateDeviceNormal(void);
extern void SOA_Wiper_UpdateWiperStatus(void);

#endif // __API_SOA_WIPER_HPP__