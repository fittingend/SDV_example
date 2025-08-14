//
//  batterymonitor_types.h
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.231
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Fri Oct  4 14:16:09 2024
//
//  Target selection: autosar_adaptive.tlc
//  Embedded hardware selection: Intel->x86-64 (Windows64)
//  Code generation objectives: Unspecified
//  Validation result: Not run


#ifndef batterymonitor_types_h_
#define batterymonitor_types_h_
#include "bms/input/impl_type_inputdata.h"
#include "msg/info/impl_type_t_msginfo.h"
#include "bms/output/impl_type_outputdata.h"
#include <stdint.h>
#include <array>
#ifndef DEFINED_TYPEDEF_FOR_struct_vblXfBXnwDP7fSuX2SM1DB_
#define DEFINED_TYPEDEF_FOR_struct_vblXfBXnwDP7fSuX2SM1DB_

struct struct_vblXfBXnwDP7fSuX2SM1DB
{
  uint8_t signal1__signal_1_;
  uint8_t signal2__signal_2_;
  uint8_t signal3__signal_3_;
  std::array<uint8_t, 100> Ydata__signal_4_;
  uint8_t signal5__signal_5_;
  std::array<uint8_t, 20> MAXdata__signal_6_;
  std::array<uint8_t, 2> MAXdata__signal_7_;
  std::array<uint8_t, 2> MINdata__signal_8_;
};

#endif
#endif                                 // batterymonitor_types_h_
