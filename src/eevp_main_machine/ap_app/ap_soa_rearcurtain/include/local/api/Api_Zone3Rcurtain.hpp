#ifndef __ZONE_3_API_LOCAL_H__
#define __ZONE_3_API_LOCAL_H__

#include <Common.h>
#include <SoaDataType.h>

extern void Zone3_RearCurtain_Init(void);
extern void Zone3_RearCurtain_Term(void);
extern void Zone3_RearCurtain_PowerOn(PowerState_e power);
extern void Zone3_RearCurtain_MovePosition(RCtnPos_e pos);
extern void Zone3_RearCurtain_UpdateContext(void);

#endif