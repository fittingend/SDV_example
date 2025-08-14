#ifndef __API_ZONE4_WIPER_HPP__
#define __API_ZONE4_WIPER_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void Zone4_Wiper_Init(void);
extern void Zone4_Wiper_Term(void);
extern void Zone4_Wiper_ControlWiper(WiperOpMode_e opMode);
extern void Zone4_Wiper_UpdateContext(void);

#endif // __API_ZONE4_WIPER_HPP__