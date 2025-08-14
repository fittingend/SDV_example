#ifndef __API_SOA_DRIVER_SEAT_HPP__
#define __API_SOA_DRIVER_SEAT_HPP__

#include <Common.h>
#include <SoaDriverSeatSkeletonImpl.hpp>
#include <SoaDataType.h>

extern void SOA_Drvseat_Init(void);
extern void SOA_Drvseat_Term(void);
extern void SOA_Drvseat_SetHeight(const DriverSeatDir_e dir);
extern void SOA_Drvseat_SetRecline(const DriverSeatDir_e dir);
extern void SOA_Drvseat_SetSlide(const DriverSeatDir_e dir);
extern void SOA_Drvseat_SetTilt(const DriverSeatDir_e dir);
extern void SOA_Drvseat_UpdateDeviceNormal();
extern void SOA_Drvseat_UpdateMotorDirection();
extern void SOA_Drvseat_UpdateMotorPosition();

#endif // __API_SOA_DRIVER_SEAT_HPP__