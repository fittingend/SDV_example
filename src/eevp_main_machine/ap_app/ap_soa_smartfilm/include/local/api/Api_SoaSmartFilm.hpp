#ifndef __API_SOA_SMARTFILM_HPP__
#define __API_SOA_SMARTFILM_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void SOA_SMARTFILM_Init();
extern void SOA_SMARTFILM_Term();

extern void SOA_SMARTFILM_SetOpacity(ClientLevel_e level, SmtfilmPos_e pos, int opacity);
extern void SOA_SMARTFILM_SetAllOpacity(ClientLevel_e level, int opacity);
extern void SOA_SMARTFILM_SetAuto(ClientLevel_e level, bool on);

extern void SOA_SMARTFILM_UpdateDeviceNormal();
extern void SOA_SMARTFILM_UpdateSmtfilmAuto();
extern void SOA_SMARTFILM_UpdateSmtfilmOpacities();

#endif // __API_SOA_SMARTFILM_HPP__
