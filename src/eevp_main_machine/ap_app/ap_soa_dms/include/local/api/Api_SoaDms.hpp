#ifndef __API_SOA_DMS_HPP__
#define __API_SOA_DMS_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void SOA_DMS_Init(void);
extern void SOA_DMS_Term(void);
extern void SOA_DMS_SetPower(ClientLevel_e level, bool on);
extern void SOA_DMS_UpdateDeviceNormal();
extern void SOA_DMS_UpdateDmsStatus();
extern void SOA_DMS_UpdateDriverStatus();
extern void SOA_DMS_UpdateSwVersion();
extern void SOA_DMS_NotifyOnDriverNotFocusing();
extern void SOA_DMS_NotifyOnDriverNotWaken();
extern void SOA_DMS_NotifyOnDriverNotWatchingFront();


#endif // __API_SOA_DMS_HPP__
