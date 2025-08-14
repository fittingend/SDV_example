#ifndef __API_SOA_HVAC_HPP__
#define __API_SOA_HVAC_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void SOA_HVAC_Init(void);
extern void SOA_HVAC_Term(void);
extern void SOA_HVAC_SetHvacPower(ClientLevel_e level, bool on);
extern void SOA_HVAC_SetAcOnOff(ClientLevel_e level, bool on);
extern void SOA_HVAC_SetAutoTemp(ClientLevel_e level, bool on);
extern void SOA_HVAC_SetSync(ClientLevel_e level, bool on);
extern void SOA_HVAC_SetDriverTemp(ClientLevel_e level, int temperature);
extern void SOA_HVAC_SetPassengerTemp(ClientLevel_e level, int temperature);
extern void SOA_HVAC_SetBlowingForce(ClientLevel_e level, int force);
extern void SOA_HVAC_SetBlowingDirection(ClientLevel_e level, HvacBlowDir_e dir);
extern void SOA_HVAC_SetAirSource(ClientLevel_e level, HvacAirSource_e src);
extern void SOA_HVAC_SetForwardDefrostOn(ClientLevel_e level, bool on);
extern void SOA_HVAC_SetBackwardDefrostOn(ClientLevel_e level, bool on);
extern void SOA_HVAC_UpdateDeviceNormal();
extern void SOA_HVAC_UpdateHvacSetting();
extern void SOA_HVAC_UpdateHvacStatus();
extern void SOA_HVAC_UpdateHvacTemps();
extern void SOA_HVAC_UpdateSwVersion();

#endif // __API_SOA_HVAC_HPP__
