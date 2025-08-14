//
//  mul_wide_u32.cpp
//
//  Code generation for model "batterymonitor_success16_msginfocurr".
//
//  Model version              : 1.172
//  Simulink Coder version : 24.2 (R2024b) 21-Jun-2024
//  C++ source code generated on : Mon Sep 30 17:37:45 2024
//  Created for block: batterymonitor_success16_msginfocurr


#include "batterymonitor/aa/mul_wide_u32.h"
#include <stdint.h>

void mul_wide_u32(uint32_t in0, uint32_t in1, uint32_t *ptrOutBitsHi, uint32_t
                  *ptrOutBitsLo)
{
  uint32_t in0Hi;
  uint32_t in0Lo;
  uint32_t in1Hi;
  uint32_t in1Lo;
  uint32_t outBitsLo;
  uint32_t productHiLo;
  uint32_t productLoHi;
  in0Hi = in0 >> 16U;
  in0Lo = in0 & 65535U;
  in1Hi = in1 >> 16U;
  in1Lo = in1 & 65535U;
  productHiLo = in0Hi * in1Lo;
  productLoHi = in0Lo * in1Hi;
  in0Lo *= in1Lo;
  in1Lo = 0U;
  outBitsLo = (productLoHi << /*MW:OvBitwiseOk*/ 16U) + /*MW:OvCarryOk*/ in0Lo;
  if (outBitsLo < in0Lo) {
    in1Lo = 1U;
  }

  in0Lo = outBitsLo;
  outBitsLo += /*MW:OvCarryOk*/ productHiLo << /*MW:OvBitwiseOk*/ 16U;
  if (outBitsLo < in0Lo) {
    in1Lo++;
  }

  *ptrOutBitsHi = (((productLoHi >> 16U) + (productHiLo >> 16U)) + in0Hi * in1Hi)
    + in1Lo;
  *ptrOutBitsLo = outBitsLo;
}