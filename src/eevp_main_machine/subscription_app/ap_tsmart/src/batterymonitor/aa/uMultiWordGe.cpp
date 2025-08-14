//
//  uMultiWordGe.cpp
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.134
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Fri Sep 27 09:24:09 2024
//  Created for block: batterymonitor


#include "batterymonitor/aa/uMultiWordGe.h"
#include <stdint.h>
#include <stdbool.h>


bool uMultiWordGe(const uint32_t u1[], const uint32_t u2[], int32_t n)
{
  return uMultiWordCmp(u1, u2, n) >= 0;
}