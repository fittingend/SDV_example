#ifndef __API_ZONE1_DRVSEAT_HPP__
#define __API_ZONE1_DRVSEAT_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void Zone1_Drvseat_Init(void);
extern void Zone1_Drvseat_Term(void);
extern void Zone1_Drvseat_SetHeight(DriverSeatDir_e eDir);
extern void Zone1_Drvseat_SetRecline(DriverSeatDir_e eDir);
extern void Zone1_Drvseat_SetSlide(DriverSeatDir_e eDir);
extern void Zone1_Drvseat_SetTilt(DriverSeatDir_e eDir);
extern void Zone1_Drvseat_UpdateContext(void);

#endif