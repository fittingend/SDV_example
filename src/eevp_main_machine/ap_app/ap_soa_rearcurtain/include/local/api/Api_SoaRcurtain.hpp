#ifndef __API_RCURTAIN_LOCAL_HPP__
#define __API_RCURTAIN_LOCAL_HPP__

#include <Common.h>
#include <SoaDataType.h>

extern void SOA_REARCURTAIN_Init(void);
extern void SOA_REARCURTAIN_Term(void);
extern int SOA_REARCURTAIN_MoveMotor(RCtnMotorDir_e eDirection);
extern int SOA_REARCURTAIN_MovePosition(int percentage);
extern void SOA_REARCURTAIN_UpdateStatus();

#endif