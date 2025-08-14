//
//  uMultiWord2Double.cpp
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.134
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Fri Sep 27 09:24:09 2024
//  Created for block: batterymonitor


#include "batterymonitor/aa/uMultiWord2Double.h"
#include <cmath>
#include <stdint.h>

double uMultiWord2Double(const uint32_t u1[], int32_t n1, int32_t e1)
{
  double y;
  int32_t exp_0;
  y = 0.0;
  exp_0 = e1;
  for (int32_t i{0}; i < n1; i++) {
    y += std::ldexp(static_cast<double>(u1[i]), exp_0);
    exp_0 += 32;
  }

  return y;
}