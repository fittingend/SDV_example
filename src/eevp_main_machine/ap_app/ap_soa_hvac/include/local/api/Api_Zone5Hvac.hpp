#ifndef __API_ZONE5_HVAC_HPP__
#define __API_ZONE5_HVAC_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void Zone5_HVAC_Init(void);
extern void Zone5_HVAC_Term(void);
extern void Zone5_HVAC_UpdateContext(void);
extern void Zone5_HVAC_SetHvacPower(bool on);
extern void Zone5_HVAC_SetAcOnOff(bool on);
extern void Zone5_HVAC_SetAutoTemp(bool on);
extern void Zone5_HVAC_SetSync(bool on);
extern void Zone5_HVAC_SetDriverTemp(int temperature);
extern void Zone5_HVAC_SetPassengerTemp(int temperature);
extern void Zone5_HVAC_SetBlowingForce(int force);
extern void Zone5_HVAC_SetBlowingDirection(HvacBlowDir_e dir);
extern void Zone5_HVAC_SetAirSource(HvacAirSource_e src);
extern void Zone5_HVAC_SetForwardDefrostOn(bool on);
extern void Zone5_HVAC_SetBackwardDefrostOn(bool on);


#endif // __API_ZONE5_HVAC_HPP__
