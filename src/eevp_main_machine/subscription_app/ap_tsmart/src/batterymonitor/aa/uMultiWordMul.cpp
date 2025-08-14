//
//  uMultiWordMul.cpp
//
//  Code generation for model "batterymonitor".
//
//  Model version              : 1.134
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Fri Sep 27 09:24:09 2024
//  Created for block: batterymonitor


#include "batterymonitor/aa/uMultiWordMul.h"
#include <stdint.h>

void uMultiWordMul(const uint32_t u1[], int32_t n1, const uint32_t u2[], int32_t
                   n2, uint32_t y[], int32_t n)
{
  int32_t k;

  // Initialize output to zero
  for (k = 0; k < n; k++) {
    y[k] = 0U;
  }

  for (int32_t i{0}; i < n1; i++) {
    int32_t ni;
    uint32_t a0;
    uint32_t a1;
    uint32_t cb;
    uint32_t u1i;
    cb = 0U;
    u1i = u1[i];
    a1 = u1i >> 16U;
    a0 = u1i & 65535U;
    ni = n - i;
    ni = n2 <= ni ? n2 : ni;
    k = i;
    for (int32_t j{0}; j < ni; j++) {
      uint32_t b1;
      uint32_t w01;
      uint32_t w10;
      uint32_t yk;
      u1i = u2[j];
      b1 = u1i >> 16U;
      u1i &= 65535U;
      w10 = a1 * u1i;
      w01 = a0 * b1;
      yk = y[k] + cb;
      cb = (yk < cb);
      u1i *= a0;
      yk += u1i;
      cb += (yk < u1i);
      u1i = w10 << 16U;
      yk += u1i;
      cb += (yk < u1i);
      u1i = w01 << 16U;
      yk += u1i;
      cb += (yk < u1i);
      y[k] = yk;
      cb += w10 >> 16U;
      cb += w01 >> 16U;
      cb += a1 * b1;
      k++;
    }

    if (k < n) {
      y[k] = cb;
    }
  }
}