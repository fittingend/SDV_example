//  mul_u32_sat.cpp
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.222
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Wed Oct  2 22:19:09 2024
//  Created for block: batterymonitor


#include "batterymonitor/aa/mul_u32_sat.h"
#include "batterymonitor/aa/mul_wide_u32.h"
#include <stdint.h>

uint32_t mul_u32_sat(uint32_t a, uint32_t b)
{
  uint32_t result;
  uint32_t u32_chi;
  mul_wide_u32(a, b, &u32_chi, &result);
  if (u32_chi) {
    result = UINT32_MAX;
  }

  return result;
}