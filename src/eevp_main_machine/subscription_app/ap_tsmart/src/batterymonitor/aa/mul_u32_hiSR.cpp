//
//  mul_u32_hiSR.cpp
//
//  Code generation for model "batterymonitor_success16_msginfocurr".
//
//  Model version              : 1.172
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Mon Sep 30 17:37:45 2024
//  Created for block: batterymonitor_success16_msginfocurr


#include "batterymonitor/aa/mul_u32_hiSR.h"
#include <stdint.h>

uint32_t mul_u32_hiSR(uint32_t a, uint32_t b, uint32_t aShift)
{
  uint32_t u32_chi;
  uint32_t u32_clo;
  mul_wide_u32(a, b, &u32_chi, &u32_clo);
  return u32_chi >> aShift;
}