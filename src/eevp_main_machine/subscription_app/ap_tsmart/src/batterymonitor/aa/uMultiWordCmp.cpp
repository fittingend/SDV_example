//
//  uMultiWordCmp.cpp
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.134
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Fri Sep 27 09:24:09 2024
//  Created for block: batterymonitor


#include "batterymonitor/aa/uMultiWordCmp.h"
#include <stdint.h>


int32_t uMultiWordCmp(const uint32_t u1[], const uint32_t u2[], int32_t n)
{
  int32_t i;
  int32_t y;
  y = 0;
  i = n;
  while ((y == 0) && (i > 0)) {
    uint32_t u1i;
    uint32_t u2i;
    i--;
    u1i = u1[i];
    u2i = u2[i];
    if (u1i != u2i) {
      y = u1i > u2i ? 1 : -1;
    }
  }

  return y;
}