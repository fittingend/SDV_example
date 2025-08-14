//
//  mod_USSjf2FO.cpp
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.156
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Mon Sep 30 11:53:24 2024
//  Created for block: batterymonitor


#include <stdint.h>
#include "batterymonitor/aa/mod_USSjf2FO.h"
#include <cmath>

// Function for MATLAB Function: '<S17>/Calculate_CurrInfo'
uint8_t mod_USSjf2FO(double x, uint8_t y)
{
  double b_r;
  uint8_t r;
  b_r = std::round(x);
  if (b_r < 256.0) {
    if (b_r >= 0.0) {
      r = static_cast<uint8_t>(b_r);
    } else {
      r = 0U;
    }
  } else {
    r = UINT8_MAX;
  }

  if (r == x) {
    if (y != 0) {
      r = static_cast<uint8_t>(r - static_cast<uint8_t>(static_cast<uint32_t>(r)
        / y * y));
    }
  } else {
    b_r = x;
    if (y == 0) {
      if (x == 0.0) {
        b_r = 0.0;
      }
    } else if (x == 0.0) {
      b_r = 0.0;
    } else {
      b_r = std::fmod(x, static_cast<double>(y));
      if (b_r == 0.0) {
        b_r = 0.0;
      } else if (b_r < 0.0) {
        b_r += static_cast<double>(y);
      }
    }

    b_r = std::round(b_r);
    if (b_r < 256.0) {
      if (b_r >= 0.0) {
        r = static_cast<uint8_t>(b_r);
      } else {
        r = 0U;
      }
    } else {
      r = UINT8_MAX;
    }
  }

  return r;
}