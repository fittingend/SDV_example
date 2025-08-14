//
//  batterymonitor.cpp
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


#include "batterymonitor/aa/batterymonitor.h"
#include <stdint.h>
#include <cmath>
#include <cstring>
// #include "mod_USSjf2FO.h"
#include <stdbool.h>
#include "bms/output/impl_type_outputdata.h"
#include <array>
// #include "multiword_types.h"
// #include "batterymonitor_private.h"
// #include "mul_u32_hiSR.h"
// #include "mul_u32_sat.h"
// #include "uMultiWordMul.h"
// #include "uMultiWordGe.h"
// #include "uMultiWordLe.h"
// #include "uMultiWord2Double.h"
// #include "zero_crossing_types.h"
#include <ctime> //시간계산

//시간계산 변수&서브함수 시작
time_t now;
time_t finishtime;
time_t starttime;
time_t elapsedtime;

int toBCD(int num) {
    //예외처리
    if (num < 0 || num > 99) return 0;

    // 10의 자리와 1의 자리를 BCD 형식으로 변환
    int bcd = ((num / 10) << 4) | (num % 10);
    return bcd;
}
//시간계산 변수&서브함수 끝

//
// Output and update for atomic system:
//    '<S17>/Convert_CurrMAXdata'
//    '<S17>/Convert_SOCMAXdata1'
//    '<S17>/Convert_TempMAXdata'
//    '<S17>/Convert_VoltMAXdata'
//
void batterymonitor::batterymonitor_Convert_CurrMAXdata(const double rtu_u[2],
  uint8_t rty_MAXdata[2])
{
  double tmp;
  tmp = std::round(rtu_u[0]);
  if (tmp < 256.0) {
    if (tmp >= 0.0) {
      rty_MAXdata[0] = static_cast<uint8_t>(tmp);
    } else {
      rty_MAXdata[0] = 0U;
    }
  } else {
    rty_MAXdata[0] = UINT8_MAX;
  }

  tmp = std::round(rtu_u[1]);
  if (tmp < 256.0) {
    if (tmp >= 0.0) {
      rty_MAXdata[1] = static_cast<uint8_t>(tmp);
    } else {
      rty_MAXdata[1] = 0U;
    }
  } else {
    rty_MAXdata[1] = UINT8_MAX;
  }
}

//
// Output and update for atomic system:
//    '<S17>/Convert_CurrMINdata'
//    '<S17>/Convert_SOCMINdata1'
//    '<S17>/Convert_TempMINdata'
//    '<S17>/Convert_VoltMINdata'
//
void batterymonitor::batterymonitor_Convert_CurrMINdata()
{
}

//
// Output and update for atomic system:
//    '<S17>/Convert_CurrRCdata'
//    '<S17>/Convert_SOCRCdata'
//    '<S17>/Convert_TempRCdata'
//    '<S17>/Convert_VoltRCdata'
//
void batterymonitor::batterymonitor_Convert_CurrRCdata(const double rtu_u[20],
  uint8_t rty_MAXdata[20])
{
  for (int32_t i{0}; i < 20; i++) {
    double tmp;
    tmp = std::round(rtu_u[i]);
    if (tmp < 256.0) {
      if (tmp >= 0.0) {
        rty_MAXdata[i] = static_cast<uint8_t>(tmp);
      } else {
        rty_MAXdata[i] = 0U;
      }
    } else {
      rty_MAXdata[i] = UINT8_MAX;
    }
  }
}

//
// Output and update for atomic system:
//    '<S17>/Convert_CurrYdata'
//    '<S17>/Convert_SOCYdata1'
//    '<S17>/Convert_TempYdata'
//    '<S17>/Convert_VoltYdata'
//
void batterymonitor::batterymonitor_Convert_CurrYdata(const double rtu_u[100],
  uint8_t rty_Ydata[100])
{
  for (int32_t i{0}; i < 100; i++) {
    double tmp;
    tmp = std::round(rtu_u[i]);
    if (tmp < 256.0) {
      if (tmp >= 0.0) {
        rty_Ydata[i] = static_cast<uint8_t>(tmp);
      } else {
        rty_Ydata[i] = 0U;
      }
    } else {
      rty_Ydata[i] = UINT8_MAX;
    }
  }
}

//
// Output and update for atomic system:
//    '<S17>/MATLAB Function'
//    '<S17>/MATLAB Function1'
//    '<S17>/MATLAB Function2'
//
void batterymonitor::batterymonitor_MATLABFunction(uint8_t rtu_y_prev, uint8_t
  rtu_val, double rtu_val_min, double rtu_val_max, uint8_t rtu_der, double
  rtu_der_min, double rtu_der_max, double rtu_ChargeStatus, double
  rtu_ChargeStatus_prev, double *rty_y)
{
  *rty_y = 0.0;
  if ((rtu_ChargeStatus != rtu_ChargeStatus_prev) && (rtu_ChargeStatus == 1.0))
  {
    *rty_y = 0.0;
  } else if (rtu_y_prev == 1) {
    *rty_y = 1.0;
  } else if ((rtu_val < rtu_val_min) || (rtu_val > rtu_val_max)) {
    *rty_y = 1.0;
  } else if ((rtu_der < rtu_der_min) || (rtu_der > rtu_der_max)) {
    *rty_y = 1.0;
  }
}

//
// Output and update for atomic system:
//    '<S17>/MATLAB Function10'
//    '<S17>/MATLAB Function11'
//    '<S17>/MATLAB Function8'
//    '<S17>/MATLAB Function9'
//
void batterymonitor::batterymonitor_MATLABFunction10(uint8_t rtu_der, double
  rtu_der_min, double rtu_der_max, double *rty_RCActive)
{
  *rty_RCActive = 0.0;
  if ((rtu_der < rtu_der_min) || (rtu_der > rtu_der_max)) {
    *rty_RCActive = 1.0;
  }
}

//
// Output and update for atomic system:
//    '<S17>/MATLAB Function4'
//    '<S17>/MATLAB Function5'
//    '<S17>/MATLAB Function6'
//
void batterymonitor::batterymonitor_MATLABFunction4(uint32_t rtu_t, uint32_t
  rtu_t_prev, uint8_t rtu_val, uint8_t rtu_val_prev, double *rty_y)
{
  if (rtu_t == rtu_t_prev) {
    *rty_y = 0.0;
  } else {
    *rty_y = static_cast<double>(rtu_val - rtu_val_prev) / (static_cast<double>
      (rtu_t) - static_cast<double>(rtu_t_prev));
  }
}

void batterymonitor::R_BmsInfoems_BmsInfoReceive(ara::com::SamplePtr< battery::
  bmsinfo::proxy::events::ems_BmsInfo::SampleType const > elementPtr)
{
  // Outputs for Function Call SubSystem: '<Root>/BatteryMonitor_triggered_sys' incorporates:
  //   TriggerPort: '<S1>/RxTriggered'

  batterymonitor_B.RxTriggered = *elementPtr;
}

// Model step function
void batterymonitor::BatteryMonitor_triggered_sys()
{
  // mLogger.LogInfo() << __func__;
  std::array<uint8_t, 20> rtb_RCData_l;
  std::array<uint8_t, 100> rtb_Ydata_cq;
  std::shared_ptr<ara::core::Result<size_t>> resultPtr;
  bms::output::OutputData expl_temp;
  uint64m_T rtb_Gain1;
  double rtb_miss;
  double rtb_y;
  int32_t i;
  int32_t rtb_get;
  int32_t rtb_start;
  uint32_t b_bu;
  uint32_t bu;
  uint32_t qY;
  uint32_t rtb_count;
  uint32_t rtb_count_a;
  uint32_t rtb_count_cl;
  uint32_t rtb_count_h;
  uint32_t rtb_y_b;
  uint32_t rtb_y_m;
  uint16_t rtb_DataTypeConversion2_g;
  uint16_t rtb_DataTypeConversion3;
  uint8_t rtb_Abs;
  uint8_t rtb_ChargeStatus;
  uint8_t rtb_Curr_Derivative;
  uint8_t rtb_MAXdata_f_idx_0;
  uint8_t rtb_MAXdata_f_idx_1;
  uint8_t rtb_MINdata_h_idx_1;
  uint8_t rtb_MINdata_idx_1;
  uint8_t rtb_MINdata_l_idx_0;
  uint8_t rtb_MINdata_l_idx_1;
  uint8_t rtb_RCValidCnt_k;
  uint8_t rtb_Temp_Derivative;
  uint8_t rtb_XaxisScale_i;
  bool rtb_FixPtRelationalOperator;
  bool rtb_FixPtRelationalOperator_k;
  // mLogger.LogInfo() << "MsgInfo 전송";
  // P_MsgInfo->ems_MsgInfo.Send(expl_temp);
  // mLogger.LogInfo() << "MoodLampBrightness 전송";
  // R_LmpCtrl->RequestMImSetBrightness(9U);

  static const uint64m_T tmp{ { 0U, 600U }// chunks
  };

  static const uint64m_T tmp_0{ { 0U, 0U }// chunks
  };

  static const uint64m_T tmp_1{ { 0U, 40U }// chunks
  };

  static const uint64m_T tmp_2{ { 0U, 80U }// chunks
  };

  static const uint64m_T tmp_3{ { 0U, 120U }// chunks
  };

  static const uint64m_T tmp_4{ { 0U, 160U }// chunks
  };

  static const uint64m_T tmp_5{ { 0U, 200U }// chunks
  };

  static const uint64m_T tmp_6{ { 0U, 280U }// chunks
  };

  static const uint64m_T tmp_7{ { 0U, 360U }// chunks
  };

  static const uint64m_T tmp_8{ { 0U, 440U }// chunks
  };

  static const uint64m_T tmp_9{ { 0U, 520U }// chunks
  };

  static const uint64m_T tmp_a{ { 0U, 584U }// chunks
  };

  static const std::array<int8_t, 12> c{ { 0, 5, 10, 15, 20, 25, 35, 45, 55, 65,
      73, 75 } };

  static const std::array<double, 12> b{ { 0.0, 28.7, 57.3, 86.0, 114.7, 143.3,
      200.7, 258.0, 315.3, 372.7, 418.5, 430.0 } };

  int32_t exitg1;
  if (R_BmsInfo) {
    resultPtr = std::make_shared< ara::core::Result<size_t> >
      (R_BmsInfo->ems_BmsInfo.GetNewSamples(std::move(std::bind(&batterymonitor::
          R_BmsInfoems_BmsInfoReceive, this, std::placeholders::_1)), 1U));
    if (resultPtr->HasValue() && (resultPtr->Value() > 0)) {
      // Outputs for Function Call SubSystem: '<Root>/BatteryMonitor_triggered_sys' 
      // MATLAB Function: '<S3>/MATLAB Function'
      // Received new event data
      rtb_start = 0;
      if (((batterymonitor_B.RxTriggered.EMS_RemainHour != 0U) ||
           (batterymonitor_B.RxTriggered.EMS_RemainMin != 0U)) &&
          ((batterymonitor_B.RxTriggered.EMS_RemainHour != 31U) ||
           (batterymonitor_B.RxTriggered.EMS_RemainMin != 63U))) {
        rtb_start = 1;
      }

      // End of MATLAB Function: '<S3>/MATLAB Function'

      // RelationalOperator: '<S51>/FixPt Relational Operator' incorporates:
      //   UnitDelay: '<S51>/Delay Input1'
      //
      //  Block description for '<S51>/Delay Input1':
      //
      //   Store in Global RAM

      rtb_FixPtRelationalOperator = (rtb_start >
        batterymonitor_DW.DelayInput1_DSTATE);

      // Outputs for Triggered SubSystem: '<S1>/Subsystem' incorporates:
      //   TriggerPort: '<S5>/Trigger'

      if (rtb_FixPtRelationalOperator &&
          (batterymonitor_PrevZCX.Subsystem_Trig_ZCE != POS_ZCSIG)) {
        if (R_LmpCtrl) {
          // FunctionCaller: '<S5>/R_LmpCtrl_RequestMImSetBrightness' incorporates:
          //   Constant: '<S5>/Constant1'

          R_LmpCtrl->RequestMImSetBrightness(9U);
        }

        if (R_LmpCtrl) {
          // FunctionCaller: '<S5>/R_LmpCtrl_RequestMImSetRGBColor' incorporates:
          //   Constant: '<S5>/Constant2'

          R_LmpCtrl->RequestMImSetRGBColor(1U);
        }
      }

      batterymonitor_PrevZCX.Subsystem_Trig_ZCE = rtb_FixPtRelationalOperator;

      // End of Outputs for SubSystem: '<S1>/Subsystem'

      // RelationalOperator: '<S50>/FixPt Relational Operator' incorporates:
      //   UnitDelay: '<S50>/Delay Input1'
      //
      //  Block description for '<S50>/Delay Input1':
      //
      //   Store in Global RAM

      rtb_FixPtRelationalOperator_k =
        (batterymonitor_B.RxTriggered.EMS_HVBatt_MainRlyCloseReq <
         batterymonitor_DW.DelayInput1_DSTATE_n);

      // Outputs for Triggered SubSystem: '<S1>/Subsystem1' incorporates:
      //   TriggerPort: '<S6>/Trigger'

      if (rtb_FixPtRelationalOperator_k &&
          (batterymonitor_PrevZCX.Subsystem1_Trig_ZCE != POS_ZCSIG)) {
        if (R_LmpCtrl) {
          // FunctionCaller: '<S6>/R_LmpCtrl_RequestMImSetBrightness' incorporates:
          //   Constant: '<S6>/Constant1'

          R_LmpCtrl->RequestMImSetBrightness(0U);
        }

        if (R_LmpCtrl) {
          // FunctionCaller: '<S6>/R_LmpCtrl_RequestMImSetRGBColor' incorporates:
          //   Constant: '<S6>/Constant2'

          R_LmpCtrl->RequestMImSetRGBColor(3U);
        }
      }

      batterymonitor_PrevZCX.Subsystem1_Trig_ZCE = rtb_FixPtRelationalOperator_k;

      // End of Outputs for SubSystem: '<S1>/Subsystem1'

      // MATLAB Function: '<S2>/Determine_AppVersion'
      expl_temp.AppVer[0] = 0U;
      expl_temp.AppVer[1] = 1U;
      expl_temp.AppVer[2] = 0U;
      expl_temp.AppVer[3] = 1U;
      expl_temp.AppVer[4] = 0U;
      expl_temp.AppVer[5] = 1U;

      // MATLAB Function: '<S2>/Determine_ChargeStatus' incorporates:
      //   UnitDelay: '<S2>/Unit Delay3'

      if (batterymonitor_DW.UnitDelay3_DSTATE == 2.0) {
        rtb_ChargeStatus = 0U;
      } else {
        rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE);
        if (rtb_y < 256.0) {
          if (rtb_y >= 0.0) {
            rtb_ChargeStatus = static_cast<uint8_t>(rtb_y);
          } else {
            rtb_ChargeStatus = 0U;
          }
        } else {
          rtb_ChargeStatus = UINT8_MAX;
        }
      }

      if (rtb_FixPtRelationalOperator) {
        rtb_ChargeStatus = 1U;
      } else if (rtb_FixPtRelationalOperator_k) {
        rtb_ChargeStatus = 2U;
      }

      // End of MATLAB Function: '<S2>/Determine_ChargeStatus'

      // MATLAB Function: '<S2>/MATLAB Function2' incorporates:
      //   UnitDelay: '<S2>/Unit Delay'

      rtb_get = 0;
      rtb_miss = 0.0;
      if ((rtb_ChargeStatus == 1) || (rtb_ChargeStatus == 2)) {
        qY = batterymonitor_DW.UnitDelay_DSTATE + /*MW:OvSatOk*/ 1U;
        if (batterymonitor_DW.UnitDelay_DSTATE + 1U <
            batterymonitor_DW.UnitDelay_DSTATE) {
          qY = UINT32_MAX;
        }

        if (batterymonitor_B.RxTriggered.DataSerialNumber == qY) {
          rtb_get = 1;
        } else if (batterymonitor_B.RxTriggered.DataSerialNumber !=
                   batterymonitor_DW.UnitDelay_DSTATE) {
          rtb_miss = (static_cast<double>
                      (batterymonitor_B.RxTriggered.DataSerialNumber) -
                      static_cast<double>(batterymonitor_DW.UnitDelay_DSTATE)) -
            1.0;
        }
      }

      // End of MATLAB Function: '<S2>/MATLAB Function2'

      // MATLAB Function: '<S2>/Calculate_GetCount' incorporates:
      //   MATLAB Function: '<S2>/Calculate_MissCount'
      //   UnitDelay: '<S2>/Unit Delay1'
      //   UnitDelay: '<S2>/Unit Delay2'

      if (rtb_ChargeStatus == 0) {
        rtb_y_m = 0U;
        rtb_y_b = 0U;
      } else {
        rtb_y = static_cast<double>(batterymonitor_DW.UnitDelay1_DSTATE) +
          static_cast<double>(rtb_get);
        if (rtb_y < 4.294967296E+9) {
          rtb_y_m = static_cast<uint32_t>(rtb_y);
        } else {
          rtb_y_m = UINT32_MAX;
        }

        rtb_y = static_cast<double>(batterymonitor_DW.UnitDelay2_DSTATE_a) +
          rtb_miss;
        if (rtb_y < 4.294967296E+9) {
          if (rtb_y >= 0.0) {
            rtb_y_b = static_cast<uint32_t>(rtb_y);
          } else {
            rtb_y_b = 0U;
          }
        } else {
          rtb_y_b = UINT32_MAX;
        }

        // Outputs for Enabled SubSystem: '<S2>/Subsystem1' incorporates:
        //   EnablePort: '<S17>/Enable'

        // Sum: '<S17>/Minus' incorporates:
        //   Constant: '<S17>/Constant'
        //   Gain: '<S17>/Gain'
        //   UnitDelay: '<S2>/Unit Delay1'
        //   UnitDelay: '<S2>/Unit Delay2'

        rtb_get = static_cast<int32_t>(mul_u32_hiSR(3435973837U,
          batterymonitor_B.RxTriggered.EMS_BattCurr, 3U)) - 1000;

        // Abs: '<S17>/Abs'
        if (rtb_get < 0) {
          rtb_Abs = static_cast<uint8_t>(-rtb_get);
        } else {
          rtb_Abs = static_cast<uint8_t>(rtb_get);
        }

        // End of Abs: '<S17>/Abs'

        // MATLAB Function: '<S17>/MATLAB Function4' incorporates:
        //   UnitDelay: '<S17>/Unit Delay28'
        //   UnitDelay: '<S17>/Unit Delay29'

        batterymonitor_MATLABFunction4
          (batterymonitor_B.RxTriggered.DataSerialNumber,
           batterymonitor_DW.UnitDelay28_DSTATE, rtb_Abs,
           batterymonitor_DW.UnitDelay29_DSTATE, &rtb_miss);

        // Abs: '<S17>/Curr_Derivative'
        if (rtb_miss < 0.0) {
          rtb_Curr_Derivative = static_cast<uint8_t>(std::fmod(std::ceil
            (-rtb_miss), 256.0));
        } else {
          rtb_Curr_Derivative = static_cast<uint8_t>(std::fmod(std::ceil
            (rtb_miss), 256.0));
        }

        // End of Abs: '<S17>/Curr_Derivative'

        // MATLAB Function: '<S17>/MATLAB Function8' incorporates:
        //   Constant: '<S17>/RCMAXDerivative_Curr'
        //   Constant: '<S17>/RCMINDerivative_Curr'

        batterymonitor_MATLABFunction10(rtb_Curr_Derivative, -10.0, 10.0,
          &rtb_miss);

        // MATLAB Function: '<S17>/Calculate_CurrInfo' incorporates:
        //   UnitDelay: '<S17>/Unit Delay10'
        //   UnitDelay: '<S17>/Unit Delay11'
        //   UnitDelay: '<S17>/Unit Delay12'
        //   UnitDelay: '<S17>/Unit Delay13'
        //   UnitDelay: '<S17>/Unit Delay36'
        //   UnitDelay: '<S17>/Unit Delay37'
        //   UnitDelay: '<S17>/Unit Delay7'
        //   UnitDelay: '<S17>/Unit Delay8'
        //   UnitDelay: '<S17>/Unit Delay9'

        std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
        for (i = 0; i < 20; i++) {
          rtb_RCData_l[i] = 0U;
        }

        if ((rtb_ChargeStatus != batterymonitor_DW.UnitDelay7_DSTATE) &&
            (rtb_ChargeStatus == 1)) {
          rtb_count_h = 0U;
          rtb_XaxisScale_i = 0U;
          rtb_Temp_Derivative = 0U;
          std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
          rtb_RCValidCnt_k = 0U;
          for (i = 0; i < 20; i++) {
            rtb_RCData_l[i] = 0U;
          }

          rtb_MAXdata_f_idx_0 = 0U;
          rtb_MINdata_l_idx_0 = UINT8_MAX;
          rtb_MAXdata_f_idx_1 = 0U;
          rtb_MINdata_l_idx_1 = UINT8_MAX;
        } else if (rtb_ChargeStatus == 2) {
          rtb_y = std::round(batterymonitor_DW.UnitDelay13_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              rtb_count_h = static_cast<uint32_t>(rtb_y);
            } else {
              rtb_count_h = 0U;
            }
          } else {
            rtb_count_h = UINT32_MAX;
          }

          rtb_XaxisScale_i = batterymonitor_DW.UnitDelay11_DSTATE;
          rtb_Temp_Derivative = batterymonitor_DW.UnitDelay8_DSTATE;
          for (i = 0; i < 100; i++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay12_DSTATE[i]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_Ydata_cq[i] = 0U;
              }
            } else {
              rtb_Ydata_cq[i] = UINT8_MAX;
            }
          }

          rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay36_DSTATE;
          for (rtb_get = 0; rtb_get < 20; rtb_get++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[rtb_get]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_RCData_l[rtb_get] = 0U;
              }
            } else {
              rtb_RCData_l[rtb_get] = UINT8_MAX;
            }
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay9_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MAXdata_f_idx_0 = 0U;
            }
          } else {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay10_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_0 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay9_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MAXdata_f_idx_1 = 0U;
            }
          } else {
            rtb_MAXdata_f_idx_1 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay10_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_1 = UINT8_MAX;
          }
        } else {
          rtb_y = std::round(batterymonitor_DW.UnitDelay13_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              qY = static_cast<uint32_t>(rtb_y);
            } else {
              qY = 0U;
            }
          } else {
            qY = UINT32_MAX;
          }

          rtb_count_h = qY + /*MW:OvSatOk*/ 1U;
          if (qY + 1U < qY) {
            rtb_count_h = UINT32_MAX;
          }

          if (batterymonitor_DW.UnitDelay11_DSTATE > 7) {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          } else {
            rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
              batterymonitor_DW.UnitDelay11_DSTATE);
          }

          if (mod_USSjf2FO(batterymonitor_DW.UnitDelay13_DSTATE,
                           rtb_MAXdata_f_idx_0) == 0) {
            if (batterymonitor_DW.UnitDelay8_DSTATE == 100) {
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay11_DSTATE + 1U);
              if (batterymonitor_DW.UnitDelay11_DSTATE + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_XaxisScale_i = static_cast<uint8_t>(rtb_get);
              for (rtb_get = 0; rtb_get < 50; rtb_get++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay12_DSTATE
                                   [((rtb_get + 1) << 1) - 2]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[rtb_get] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[rtb_get] = UINT8_MAX;
                }
              }

              rtb_Temp_Derivative = 51U;
              rtb_Ydata_cq[50] = rtb_Abs;
            } else {
              rtb_XaxisScale_i = batterymonitor_DW.UnitDelay11_DSTATE;
              rtb_get = static_cast<int32_t>(batterymonitor_DW.UnitDelay8_DSTATE
                + 1U);
              if (batterymonitor_DW.UnitDelay8_DSTATE + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_Temp_Derivative = static_cast<uint8_t>(rtb_get);
              for (i = 0; i < 100; i++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay12_DSTATE[i]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[i] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[i] = UINT8_MAX;
                }
              }

              rtb_Ydata_cq[static_cast<uint8_t>(rtb_get) - 1] = rtb_Abs;
            }
          } else {
            rtb_XaxisScale_i = batterymonitor_DW.UnitDelay11_DSTATE;
            rtb_Temp_Derivative = batterymonitor_DW.UnitDelay8_DSTATE;
            for (rtb_get = 0; rtb_get < 100; rtb_get++) {
              rtb_y = std::round(batterymonitor_DW.UnitDelay12_DSTATE[rtb_get]);
              if (rtb_y < 256.0) {
                if (rtb_y >= 0.0) {
                  rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                } else {
                  rtb_Ydata_cq[rtb_get] = 0U;
                }
              } else {
                rtb_Ydata_cq[rtb_get] = UINT8_MAX;
              }
            }
          }

          if (batterymonitor_DW.UnitDelay13_DSTATE == 0.0) {
            rtb_count_h = 1U;
            rtb_RCValidCnt_k = 0U;
            for (i = 0; i < 20; i++) {
              rtb_RCData_l[i] = 0U;
            }

            rtb_MAXdata_f_idx_0 = 0U;
            rtb_MAXdata_f_idx_1 = rtb_Abs;
            rtb_MINdata_l_idx_0 = 0U;
            rtb_MINdata_l_idx_1 = rtb_Abs;
          } else {
            if (batterymonitor_DW.UnitDelay11_DSTATE > 7) {
              rtb_MAXdata_f_idx_0 = UINT8_MAX;
            } else {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
                batterymonitor_DW.UnitDelay11_DSTATE);
            }

            if ((mod_USSjf2FO(batterymonitor_DW.UnitDelay13_DSTATE,
                              rtb_MAXdata_f_idx_0) == 0) &&
                (batterymonitor_DW.UnitDelay8_DSTATE == 100)) {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay36_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay36_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay36_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCValidCnt_k = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay37_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }

                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = 50U;
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = rtb_Abs;
                } else {
                  rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay36_DSTATE;
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay37_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay36_DSTATE;
                if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }

                for (i = 0; i < 10; i++) {
                  rtb_get = ((i + 1) << 1) - 2;
                  rtb_y = std::round
                    (batterymonitor_DW.UnitDelay37_DSTATE[rtb_get] / 2.0);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[rtb_get] = 0U;
                    }
                  } else {
                    rtb_RCData_l[rtb_get] = UINT8_MAX;
                  }
                }
              }

              if (rtb_Abs >= batterymonitor_DW.UnitDelay9_DSTATE[1]) {
                rtb_MAXdata_f_idx_0 = 50U;
                rtb_MAXdata_f_idx_1 = rtb_Abs;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay9_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay9_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_1 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_1 = UINT8_MAX;
                }
              }

              if (rtb_Abs <= batterymonitor_DW.UnitDelay10_DSTATE[1]) {
                rtb_MINdata_l_idx_0 = 50U;
                rtb_MINdata_l_idx_1 = rtb_Abs;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay10_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_0 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay10_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }
              }
            } else {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay36_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay36_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay36_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCValidCnt_k = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_get = 255;
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  qY = rtb_Temp_Derivative -
                    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                    /*MW:OvSatOk*/ 1U;
                  if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                    qY = 0U;
                  }

                  rtb_RCData_l[rtb_get - 1] = static_cast<uint8_t>(qY);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = rtb_Abs;
                } else {
                  rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay36_DSTATE;
                  if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay36_DSTATE;
                if (batterymonitor_DW.UnitDelay36_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay36_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay37_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }
              }

              if (rtb_Abs >= batterymonitor_DW.UnitDelay9_DSTATE[1]) {
                qY = rtb_Temp_Derivative - 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(qY);
                rtb_MAXdata_f_idx_1 = rtb_Abs;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay9_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay9_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_1 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_1 = UINT8_MAX;
                }
              }

              if (rtb_Abs <= batterymonitor_DW.UnitDelay10_DSTATE[1]) {
                qY = rtb_Temp_Derivative -
                  /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                  /*MW:OvSatOk*/ 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MINdata_l_idx_0 = static_cast<uint8_t>(qY);
                rtb_MINdata_l_idx_1 = rtb_Abs;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay10_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_0 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay10_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }
              }
            }
          }
        }

        // DataTypeConversion: '<S17>/Data Type Conversion7'
        batterymonitor_DW.UnitDelay9_DSTATE[0] = rtb_MAXdata_f_idx_0;
        batterymonitor_DW.UnitDelay9_DSTATE[1] = rtb_MAXdata_f_idx_1;

        // MATLAB Function: '<S17>/Convert_CurrMAXdata'
        batterymonitor_Convert_CurrMAXdata(&batterymonitor_DW.UnitDelay9_DSTATE
          [0], &batterymonitor_B.MAXdata_o[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion8'
        batterymonitor_DW.UnitDelay10_DSTATE[0] = rtb_MINdata_l_idx_0;
        batterymonitor_DW.UnitDelay10_DSTATE[1] = rtb_MINdata_l_idx_1;

        // MATLAB Function: '<S17>/Convert_CurrMINdata'
        batterymonitor_Convert_CurrMAXdata
          (&batterymonitor_DW.UnitDelay10_DSTATE[0],
           &batterymonitor_B.MINdata_iq[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion29'
        for (i = 0; i < 20; i++) {
          batterymonitor_DW.UnitDelay37_DSTATE[i] = rtb_RCData_l[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion29'

        // MATLAB Function: '<S17>/Convert_CurrRCdata'
        batterymonitor_Convert_CurrRCdata(&batterymonitor_DW.UnitDelay37_DSTATE
          [0], &batterymonitor_B.MAXdata_b[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion6'
        for (i = 0; i < 100; i++) {
          batterymonitor_DW.UnitDelay12_DSTATE[i] = rtb_Ydata_cq[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion6'

        // MATLAB Function: '<S17>/Convert_CurrYdata'
        batterymonitor_Convert_CurrYdata(&batterymonitor_DW.UnitDelay12_DSTATE[0],
          &batterymonitor_B.Ydata_p[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion10' incorporates:
        //   UnitDelay: '<S17>/Unit Delay11'

        batterymonitor_DW.UnitDelay11_DSTATE = rtb_XaxisScale_i;

        // MATLAB Function: '<S17>/MATLAB Function' incorporates:
        //   Constant: '<S17>/MAXDerivative_Curr'
        //   Constant: '<S17>/MAXRange_Curr'
        //   Constant: '<S17>/MINDerivative_Curr'
        //   Constant: '<S17>/MINRange_Curr'
        //   DataTypeConversion: '<S17>/Data Type Conversion36'
        //   DataTypeConversion: '<S17>/Data Type Conversion37'
        //   UnitDelay: '<S17>/Unit Delay46'
        //   UnitDelay: '<S17>/Unit Delay7'

        batterymonitor_MATLABFunction(batterymonitor_DW.UnitDelay46_DSTATE,
          rtb_Abs, -10.0, 400.0, rtb_Curr_Derivative, 0.0, 500.0, static_cast<
          double>(rtb_ChargeStatus), static_cast<double>
          (batterymonitor_DW.UnitDelay7_DSTATE), &rtb_miss);

        // DataTypeConversion: '<S17>/Data Type Conversion26' incorporates:
        //   UnitDelay: '<S17>/Unit Delay46'

        rtb_y = std::fmod(std::floor(rtb_miss), 256.0);
        if (rtb_y < 0.0) {
          batterymonitor_DW.UnitDelay46_DSTATE = static_cast<uint8_t>(-
            static_cast<int8_t>(static_cast<uint8_t>(-rtb_y)));
        } else {
          batterymonitor_DW.UnitDelay46_DSTATE = static_cast<uint8_t>(rtb_y);
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion26'

        // DataTypeConversion: '<S17>/Data Type Conversion28' incorporates:
        //   UnitDelay: '<S17>/Unit Delay36'

        batterymonitor_DW.UnitDelay36_DSTATE = rtb_RCValidCnt_k;

        // DataTypeConversion: '<S17>/Data Type Conversion9' incorporates:
        //   MATLAB Function: '<S17>/Calculate_CurrInfo'
        //   UnitDelay: '<S17>/Unit Delay8'

        batterymonitor_DW.UnitDelay8_DSTATE = rtb_Temp_Derivative;

        // SignalConversion generated from: '<S17>/CurrenttRslt' incorporates:
        //   UnitDelay: '<S17>/Unit Delay46'

        batterymonitor_B.OutportBufferForCurrenttRslt =
          batterymonitor_DW.UnitDelay46_DSTATE;

        // Sum: '<S17>/Minus2' incorporates:
        //   Gain: '<S17>/Gain1'

        rtb_get = static_cast<int32_t>(mul_u32_hiSR(3435973837U,
          batterymonitor_B.RxTriggered.EMS_BattVolt, 4U));

        // Abs: '<S17>/Abs1'
        if (rtb_get < 0) {
          rtb_Curr_Derivative = static_cast<uint8_t>(-rtb_get);
        } else {
          rtb_Curr_Derivative = static_cast<uint8_t>(rtb_get);
        }

        // End of Abs: '<S17>/Abs1'

        // MATLAB Function: '<S17>/MATLAB Function5' incorporates:
        //   UnitDelay: '<S17>/Unit Delay30'
        //   UnitDelay: '<S17>/Unit Delay31'

        batterymonitor_MATLABFunction4
          (batterymonitor_B.RxTriggered.DataSerialNumber,
           batterymonitor_DW.UnitDelay30_DSTATE, rtb_Curr_Derivative,
           batterymonitor_DW.UnitDelay31_DSTATE, &rtb_miss);

        // Abs: '<S17>/Volt_Derivative'
        if (rtb_miss < 0.0) {
          rtb_XaxisScale_i = static_cast<uint8_t>(std::fmod(std::ceil(-rtb_miss),
            256.0));
        } else {
          rtb_XaxisScale_i = static_cast<uint8_t>(std::fmod(std::ceil(rtb_miss),
            256.0));
        }

        // End of Abs: '<S17>/Volt_Derivative'

        // MATLAB Function: '<S17>/MATLAB Function9' incorporates:
        //   Constant: '<S17>/RCMAXDerivative_Volt'
        //   Constant: '<S17>/RCMINDerivative_Volt'

        batterymonitor_MATLABFunction10(rtb_XaxisScale_i, -10.0, 10.0, &rtb_miss);

        // MATLAB Function: '<S17>/Calculate_VoltInfo' incorporates:
        //   MATLAB Function: '<S17>/Calculate_CurrInfo'
        //   UnitDelay: '<S17>/Unit Delay14'
        //   UnitDelay: '<S17>/Unit Delay15'
        //   UnitDelay: '<S17>/Unit Delay16'
        //   UnitDelay: '<S17>/Unit Delay17'
        //   UnitDelay: '<S17>/Unit Delay18'
        //   UnitDelay: '<S17>/Unit Delay19'
        //   UnitDelay: '<S17>/Unit Delay20'
        //   UnitDelay: '<S17>/Unit Delay38'
        //   UnitDelay: '<S17>/Unit Delay39'

        std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
        for (i = 0; i < 20; i++) {
          rtb_RCData_l[i] = 0U;
        }

        if ((rtb_ChargeStatus != batterymonitor_DW.UnitDelay14_DSTATE) &&
            (rtb_ChargeStatus == 1)) {
          rtb_count = 0U;
          rtb_RCValidCnt_k = 0U;
          rtb_Temp_Derivative = 0U;
          std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
          rtb_MAXdata_f_idx_1 = 0U;
          for (i = 0; i < 20; i++) {
            rtb_RCData_l[i] = 0U;
          }

          rtb_MAXdata_f_idx_0 = 0U;
          rtb_MINdata_l_idx_1 = UINT8_MAX;
          rtb_MINdata_l_idx_0 = 0U;
          rtb_MINdata_idx_1 = UINT8_MAX;
        } else if (rtb_ChargeStatus == 2) {
          rtb_y = std::round(batterymonitor_DW.UnitDelay20_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              rtb_count = static_cast<uint32_t>(rtb_y);
            } else {
              rtb_count = 0U;
            }
          } else {
            rtb_count = UINT32_MAX;
          }

          rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay18_DSTATE;
          rtb_Temp_Derivative = batterymonitor_DW.UnitDelay15_DSTATE;
          for (i = 0; i < 100; i++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay19_DSTATE[i]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_Ydata_cq[i] = 0U;
              }
            } else {
              rtb_Ydata_cq[i] = UINT8_MAX;
            }
          }

          rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay38_DSTATE;
          for (rtb_get = 0; rtb_get < 20; rtb_get++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[rtb_get]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_RCData_l[rtb_get] = 0U;
              }
            } else {
              rtb_RCData_l[rtb_get] = UINT8_MAX;
            }
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay16_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MAXdata_f_idx_0 = 0U;
            }
          } else {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay17_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_1 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay16_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_0 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay17_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_idx_1 = UINT8_MAX;
          }
        } else {
          rtb_y = std::round(batterymonitor_DW.UnitDelay20_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              qY = static_cast<uint32_t>(rtb_y);
            } else {
              qY = 0U;
            }
          } else {
            qY = UINT32_MAX;
          }

          rtb_count = qY + /*MW:OvSatOk*/ 1U;
          if (qY + 1U < qY) {
            rtb_count = UINT32_MAX;
          }

          if (batterymonitor_DW.UnitDelay18_DSTATE > 7) {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          } else {
            rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
              batterymonitor_DW.UnitDelay18_DSTATE);
          }

          if (mod_USSjf2FO(batterymonitor_DW.UnitDelay20_DSTATE,
                           rtb_MAXdata_f_idx_0) == 0) {
            if (batterymonitor_DW.UnitDelay15_DSTATE == 100) {
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay18_DSTATE + 1U);
              if (batterymonitor_DW.UnitDelay18_DSTATE + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_RCValidCnt_k = static_cast<uint8_t>(rtb_get);
              for (rtb_get = 0; rtb_get < 50; rtb_get++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay19_DSTATE
                                   [((rtb_get + 1) << 1) - 2]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[rtb_get] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[rtb_get] = UINT8_MAX;
                }
              }

              rtb_Temp_Derivative = 51U;
              rtb_Ydata_cq[50] = rtb_Curr_Derivative;
            } else {
              rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay18_DSTATE;
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay15_DSTATE + 1U);
              if (batterymonitor_DW.UnitDelay15_DSTATE + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_Temp_Derivative = static_cast<uint8_t>(rtb_get);
              for (i = 0; i < 100; i++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay19_DSTATE[i]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[i] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[i] = UINT8_MAX;
                }
              }

              rtb_Ydata_cq[static_cast<uint8_t>(rtb_get) - 1] =
                rtb_Curr_Derivative;
            }
          } else {
            rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay18_DSTATE;
            rtb_Temp_Derivative = batterymonitor_DW.UnitDelay15_DSTATE;
            for (rtb_get = 0; rtb_get < 100; rtb_get++) {
              rtb_y = std::round(batterymonitor_DW.UnitDelay19_DSTATE[rtb_get]);
              if (rtb_y < 256.0) {
                if (rtb_y >= 0.0) {
                  rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                } else {
                  rtb_Ydata_cq[rtb_get] = 0U;
                }
              } else {
                rtb_Ydata_cq[rtb_get] = UINT8_MAX;
              }
            }
          }

          if (batterymonitor_DW.UnitDelay20_DSTATE == 0.0) {
            rtb_count = 1U;
            rtb_MAXdata_f_idx_1 = 0U;
            for (i = 0; i < 20; i++) {
              rtb_RCData_l[i] = 0U;
            }

            rtb_MAXdata_f_idx_0 = 0U;
            rtb_MINdata_l_idx_0 = rtb_Curr_Derivative;
            rtb_MINdata_l_idx_1 = 0U;
            rtb_MINdata_idx_1 = rtb_Curr_Derivative;
          } else {
            if (batterymonitor_DW.UnitDelay18_DSTATE > 7) {
              rtb_MAXdata_f_idx_0 = UINT8_MAX;
            } else {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
                batterymonitor_DW.UnitDelay18_DSTATE);
            }

            if ((mod_USSjf2FO(batterymonitor_DW.UnitDelay20_DSTATE,
                              rtb_MAXdata_f_idx_0) == 0) &&
                (batterymonitor_DW.UnitDelay15_DSTATE == 100)) {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay38_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay38_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay38_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay39_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }

                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = 50U;
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = rtb_Curr_Derivative;
                } else {
                  rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay38_DSTATE;
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay39_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay38_DSTATE;
                if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }

                for (i = 0; i < 10; i++) {
                  rtb_get = ((i + 1) << 1) - 2;
                  rtb_y = std::round
                    (batterymonitor_DW.UnitDelay39_DSTATE[rtb_get] / 2.0);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[rtb_get] = 0U;
                    }
                  } else {
                    rtb_RCData_l[rtb_get] = UINT8_MAX;
                  }
                }
              }

              if (rtb_Curr_Derivative >= batterymonitor_DW.UnitDelay16_DSTATE[1])
              {
                rtb_MAXdata_f_idx_0 = 50U;
                rtb_MINdata_l_idx_0 = rtb_Curr_Derivative;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay16_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay16_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_0 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_0 = UINT8_MAX;
                }
              }

              if (rtb_Curr_Derivative <= batterymonitor_DW.UnitDelay17_DSTATE[1])
              {
                rtb_MINdata_l_idx_1 = 50U;
                rtb_MINdata_idx_1 = rtb_Curr_Derivative;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay17_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay17_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_idx_1 = UINT8_MAX;
                }
              }
            } else {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay38_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay38_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay38_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_get = 255;
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  qY = rtb_Temp_Derivative -
                    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                    /*MW:OvSatOk*/ 1U;
                  if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                    qY = 0U;
                  }

                  rtb_RCData_l[rtb_get - 1] = static_cast<uint8_t>(qY);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = rtb_Curr_Derivative;
                } else {
                  rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay38_DSTATE;
                  if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay38_DSTATE;
                if (batterymonitor_DW.UnitDelay38_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay38_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay39_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }
              }

              if (rtb_Curr_Derivative >= batterymonitor_DW.UnitDelay16_DSTATE[1])
              {
                qY = rtb_Temp_Derivative - 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(qY);
                rtb_MINdata_l_idx_0 = rtb_Curr_Derivative;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay16_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay16_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_0 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_0 = UINT8_MAX;
                }
              }

              if (rtb_Curr_Derivative <= batterymonitor_DW.UnitDelay17_DSTATE[1])
              {
                qY = rtb_Temp_Derivative -
                  /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                  /*MW:OvSatOk*/ 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MINdata_l_idx_1 = static_cast<uint8_t>(qY);
                rtb_MINdata_idx_1 = rtb_Curr_Derivative;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay17_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay17_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_idx_1 = UINT8_MAX;
                }
              }
            }
          }
        }

        // DataTypeConversion: '<S17>/Data Type Conversion13'
        batterymonitor_DW.UnitDelay16_DSTATE[0] = rtb_MAXdata_f_idx_0;
        batterymonitor_DW.UnitDelay16_DSTATE[1] = rtb_MINdata_l_idx_0;

        // MATLAB Function: '<S17>/Convert_VoltMAXdata'
        batterymonitor_Convert_CurrMAXdata
          (&batterymonitor_DW.UnitDelay16_DSTATE[0],
           &batterymonitor_B.MAXdata_m[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion14'
        batterymonitor_DW.UnitDelay17_DSTATE[0] = rtb_MINdata_l_idx_1;
        batterymonitor_DW.UnitDelay17_DSTATE[1] = rtb_MINdata_idx_1;

        // MATLAB Function: '<S17>/Convert_VoltMINdata'
        batterymonitor_Convert_CurrMAXdata
          (&batterymonitor_DW.UnitDelay17_DSTATE[0], &batterymonitor_B.MINdata[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion31'
        for (i = 0; i < 20; i++) {
          batterymonitor_DW.UnitDelay39_DSTATE[i] = rtb_RCData_l[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion31'

        // MATLAB Function: '<S17>/Convert_VoltRCdata'
        batterymonitor_Convert_CurrRCdata(&batterymonitor_DW.UnitDelay39_DSTATE
          [0], &batterymonitor_B.MAXdata[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion12'
        for (i = 0; i < 100; i++) {
          batterymonitor_DW.UnitDelay19_DSTATE[i] = rtb_Ydata_cq[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion12'

        // MATLAB Function: '<S17>/Convert_VoltYdata'
        batterymonitor_Convert_CurrYdata(&batterymonitor_DW.UnitDelay19_DSTATE[0],
          &batterymonitor_B.Ydata[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion15' incorporates:
        //   MATLAB Function: '<S17>/Calculate_VoltInfo'
        //   UnitDelay: '<S17>/Unit Delay15'

        batterymonitor_DW.UnitDelay15_DSTATE = rtb_Temp_Derivative;

        // DataTypeConversion: '<S17>/Data Type Conversion16' incorporates:
        //   UnitDelay: '<S17>/Unit Delay18'

        batterymonitor_DW.UnitDelay18_DSTATE = rtb_RCValidCnt_k;

        // MATLAB Function: '<S17>/MATLAB Function1' incorporates:
        //   Constant: '<S17>/MAXDerivative_Volt'
        //   Constant: '<S17>/MAXRange_Volt'
        //   Constant: '<S17>/MINDerivative_Volt'
        //   Constant: '<S17>/MINRange_Volt'
        //   DataTypeConversion: '<S17>/Data Type Conversion38'
        //   DataTypeConversion: '<S17>/Data Type Conversion39'
        //   UnitDelay: '<S17>/Unit Delay14'
        //   UnitDelay: '<S17>/Unit Delay47'

        batterymonitor_MATLABFunction(batterymonitor_DW.UnitDelay47_DSTATE,
          rtb_Curr_Derivative, 150.0, 250.0, rtb_XaxisScale_i, 0.0, 500.0,
          static_cast<double>(rtb_ChargeStatus), static_cast<double>
          (batterymonitor_DW.UnitDelay14_DSTATE), &rtb_miss);

        // DataTypeConversion: '<S17>/Data Type Conversion27' incorporates:
        //   UnitDelay: '<S17>/Unit Delay47'

        rtb_y = std::fmod(std::floor(rtb_miss), 256.0);
        if (rtb_y < 0.0) {
          batterymonitor_DW.UnitDelay47_DSTATE = static_cast<uint8_t>(-
            static_cast<int8_t>(static_cast<uint8_t>(-rtb_y)));
        } else {
          batterymonitor_DW.UnitDelay47_DSTATE = static_cast<uint8_t>(rtb_y);
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion27'

        // DataTypeConversion: '<S17>/Data Type Conversion30' incorporates:
        //   UnitDelay: '<S17>/Unit Delay38'

        batterymonitor_DW.UnitDelay38_DSTATE = rtb_MAXdata_f_idx_1;

        // SignalConversion generated from: '<S17>/VolttageRslt' incorporates:
        //   UnitDelay: '<S17>/Unit Delay47'

        batterymonitor_B.OutportBufferForVolttageRslt =
          batterymonitor_DW.UnitDelay47_DSTATE;

        // Sum: '<S17>/Minus1' incorporates:
        //   Gain: '<S17>/Gain2'

        rtb_get = static_cast<int32_t>(mul_u32_hiSR(3435973837U,
          batterymonitor_B.RxTriggered.EMS_HVBattSOC, 3U));

        // Abs: '<S17>/Abs2'
        if (rtb_get < 0) {
          rtb_XaxisScale_i = static_cast<uint8_t>(-rtb_get);
        } else {
          rtb_XaxisScale_i = static_cast<uint8_t>(rtb_get);
        }

        // End of Abs: '<S17>/Abs2'

        // MATLAB Function: '<S17>/MATLAB Function6' incorporates:
        //   UnitDelay: '<S17>/Unit Delay32'
        //   UnitDelay: '<S17>/Unit Delay33'

        batterymonitor_MATLABFunction4
          (batterymonitor_B.RxTriggered.DataSerialNumber,
           batterymonitor_DW.UnitDelay32_DSTATE, rtb_XaxisScale_i,
           batterymonitor_DW.UnitDelay33_DSTATE, &rtb_miss);

        // Abs: '<S17>/SOC_Derivative'
        if (rtb_miss < 0.0) {
          rtb_RCValidCnt_k = static_cast<uint8_t>(std::fmod(std::ceil(-rtb_miss),
            256.0));
        } else {
          rtb_RCValidCnt_k = static_cast<uint8_t>(std::fmod(std::ceil(rtb_miss),
            256.0));
        }

        // End of Abs: '<S17>/SOC_Derivative'

        // MATLAB Function: '<S17>/MATLAB Function10' incorporates:
        //   Constant: '<S17>/RCMAXDerivative_SOC'
        //   Constant: '<S17>/RCMINDerivative_SOC'

        batterymonitor_MATLABFunction10(rtb_RCValidCnt_k, -10.0, 10.0, &rtb_miss);

        // MATLAB Function: '<S17>/Calculate_SOCInfo' incorporates:
        //   MATLAB Function: '<S17>/Calculate_CurrInfo'
        //   UnitDelay: '<S17>/Unit Delay21'
        //   UnitDelay: '<S17>/Unit Delay22'
        //   UnitDelay: '<S17>/Unit Delay23'
        //   UnitDelay: '<S17>/Unit Delay24'
        //   UnitDelay: '<S17>/Unit Delay25'
        //   UnitDelay: '<S17>/Unit Delay26'
        //   UnitDelay: '<S17>/Unit Delay27'
        //   UnitDelay: '<S17>/Unit Delay40'
        //   UnitDelay: '<S17>/Unit Delay41'

        std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
        for (i = 0; i < 20; i++) {
          rtb_RCData_l[i] = 0U;
        }

        if ((rtb_ChargeStatus != batterymonitor_DW.UnitDelay21_DSTATE) &&
            (rtb_ChargeStatus == 1)) {
          rtb_count_cl = 0U;
          rtb_MAXdata_f_idx_1 = 0U;
          rtb_Temp_Derivative = 0U;
          std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
          rtb_MINdata_l_idx_0 = 0U;
          for (i = 0; i < 20; i++) {
            rtb_RCData_l[i] = 0U;
          }

          rtb_MAXdata_f_idx_0 = 0U;
          rtb_MINdata_idx_1 = UINT8_MAX;
          rtb_MINdata_l_idx_1 = 0U;
          rtb_MINdata_h_idx_1 = UINT8_MAX;
        } else if (rtb_ChargeStatus == 2) {
          rtb_y = std::round(batterymonitor_DW.UnitDelay27_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              rtb_count_cl = static_cast<uint32_t>(rtb_y);
            } else {
              rtb_count_cl = 0U;
            }
          } else {
            rtb_count_cl = UINT32_MAX;
          }

          rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay25_DSTATE;
          rtb_Temp_Derivative = batterymonitor_DW.UnitDelay22_DSTATE;
          for (i = 0; i < 100; i++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay26_DSTATE[i]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_Ydata_cq[i] = 0U;
              }
            } else {
              rtb_Ydata_cq[i] = UINT8_MAX;
            }
          }

          rtb_MINdata_l_idx_0 = batterymonitor_DW.UnitDelay40_DSTATE;
          for (rtb_get = 0; rtb_get < 20; rtb_get++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[rtb_get]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_RCData_l[rtb_get] = 0U;
              }
            } else {
              rtb_RCData_l[rtb_get] = UINT8_MAX;
            }
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay23_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MAXdata_f_idx_0 = 0U;
            }
          } else {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay24_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_idx_1 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay23_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_1 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay24_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_h_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_h_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_h_idx_1 = UINT8_MAX;
          }
        } else {
          rtb_y = std::round(batterymonitor_DW.UnitDelay27_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              qY = static_cast<uint32_t>(rtb_y);
            } else {
              qY = 0U;
            }
          } else {
            qY = UINT32_MAX;
          }

          rtb_count_cl = qY + /*MW:OvSatOk*/ 1U;
          if (qY + 1U < qY) {
            rtb_count_cl = UINT32_MAX;
          }

          if (batterymonitor_DW.UnitDelay25_DSTATE > 7) {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          } else {
            rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
              batterymonitor_DW.UnitDelay25_DSTATE);
          }

          if (mod_USSjf2FO(batterymonitor_DW.UnitDelay27_DSTATE,
                           rtb_MAXdata_f_idx_0) == 0) {
            if (batterymonitor_DW.UnitDelay22_DSTATE == 100) {
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay25_DSTATE + 1U);
              if (batterymonitor_DW.UnitDelay25_DSTATE + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_get);
              for (rtb_get = 0; rtb_get < 50; rtb_get++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay26_DSTATE
                                   [((rtb_get + 1) << 1) - 2]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[rtb_get] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[rtb_get] = UINT8_MAX;
                }
              }

              rtb_Temp_Derivative = 51U;
              rtb_Ydata_cq[50] = rtb_XaxisScale_i;
            } else {
              rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay25_DSTATE;
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay22_DSTATE + 1U);
              if (batterymonitor_DW.UnitDelay22_DSTATE + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_Temp_Derivative = static_cast<uint8_t>(rtb_get);
              for (i = 0; i < 100; i++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay26_DSTATE[i]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[i] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[i] = UINT8_MAX;
                }
              }

              rtb_Ydata_cq[static_cast<uint8_t>(rtb_get) - 1] = rtb_XaxisScale_i;
            }
          } else {
            rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay25_DSTATE;
            rtb_Temp_Derivative = batterymonitor_DW.UnitDelay22_DSTATE;
            for (rtb_get = 0; rtb_get < 100; rtb_get++) {
              rtb_y = std::round(batterymonitor_DW.UnitDelay26_DSTATE[rtb_get]);
              if (rtb_y < 256.0) {
                if (rtb_y >= 0.0) {
                  rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                } else {
                  rtb_Ydata_cq[rtb_get] = 0U;
                }
              } else {
                rtb_Ydata_cq[rtb_get] = UINT8_MAX;
              }
            }
          }

          if (batterymonitor_DW.UnitDelay27_DSTATE == 0.0) {
            rtb_count_cl = 1U;
            rtb_MINdata_l_idx_0 = 0U;
            for (i = 0; i < 20; i++) {
              rtb_RCData_l[i] = 0U;
            }

            rtb_MAXdata_f_idx_0 = 0U;
            rtb_MINdata_l_idx_1 = rtb_XaxisScale_i;
            rtb_MINdata_idx_1 = 0U;
            rtb_MINdata_h_idx_1 = rtb_XaxisScale_i;
          } else {
            if (batterymonitor_DW.UnitDelay25_DSTATE > 7) {
              rtb_MAXdata_f_idx_0 = UINT8_MAX;
            } else {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
                batterymonitor_DW.UnitDelay25_DSTATE);
            }

            if ((mod_USSjf2FO(batterymonitor_DW.UnitDelay27_DSTATE,
                              rtb_MAXdata_f_idx_0) == 0) &&
                (batterymonitor_DW.UnitDelay22_DSTATE == 100)) {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay40_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay40_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay40_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay41_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }

                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = 50U;
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = rtb_XaxisScale_i;
                } else {
                  rtb_MINdata_l_idx_0 = batterymonitor_DW.UnitDelay40_DSTATE;
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay41_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_MINdata_l_idx_0 = batterymonitor_DW.UnitDelay40_DSTATE;
                if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }

                for (i = 0; i < 10; i++) {
                  rtb_get = ((i + 1) << 1) - 2;
                  rtb_y = std::round
                    (batterymonitor_DW.UnitDelay41_DSTATE[rtb_get] / 2.0);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[rtb_get] = 0U;
                    }
                  } else {
                    rtb_RCData_l[rtb_get] = UINT8_MAX;
                  }
                }
              }

              if (rtb_XaxisScale_i >= batterymonitor_DW.UnitDelay23_DSTATE[1]) {
                rtb_MAXdata_f_idx_0 = 50U;
                rtb_MINdata_l_idx_1 = rtb_XaxisScale_i;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay23_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay23_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }
              }

              if (rtb_XaxisScale_i <= batterymonitor_DW.UnitDelay24_DSTATE[1]) {
                rtb_MINdata_idx_1 = 50U;
                rtb_MINdata_h_idx_1 = rtb_XaxisScale_i;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay24_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_idx_1 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay24_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_h_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_h_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_h_idx_1 = UINT8_MAX;
                }
              }
            } else {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay40_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay40_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay40_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_get = 255;
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  qY = rtb_Temp_Derivative -
                    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                    /*MW:OvSatOk*/ 1U;
                  if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                    qY = 0U;
                  }

                  rtb_RCData_l[rtb_get - 1] = static_cast<uint8_t>(qY);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = rtb_XaxisScale_i;
                } else {
                  rtb_MINdata_l_idx_0 = batterymonitor_DW.UnitDelay40_DSTATE;
                  if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_MINdata_l_idx_0 = batterymonitor_DW.UnitDelay40_DSTATE;
                if (batterymonitor_DW.UnitDelay40_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay40_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay41_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }
              }

              if (rtb_XaxisScale_i >= batterymonitor_DW.UnitDelay23_DSTATE[1]) {
                qY = rtb_Temp_Derivative - 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(qY);
                rtb_MINdata_l_idx_1 = rtb_XaxisScale_i;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay23_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay23_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }
              }

              if (rtb_XaxisScale_i <= batterymonitor_DW.UnitDelay24_DSTATE[1]) {
                qY = rtb_Temp_Derivative -
                  /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                  /*MW:OvSatOk*/ 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MINdata_idx_1 = static_cast<uint8_t>(qY);
                rtb_MINdata_h_idx_1 = rtb_XaxisScale_i;
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay24_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_idx_1 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay24_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_h_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_h_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_h_idx_1 = UINT8_MAX;
                }
              }
            }
          }
        }

        // DataTypeConversion: '<S17>/Data Type Conversion19'
        batterymonitor_DW.UnitDelay23_DSTATE[0] = rtb_MAXdata_f_idx_0;
        batterymonitor_DW.UnitDelay23_DSTATE[1] = rtb_MINdata_l_idx_1;

        // MATLAB Function: '<S17>/Convert_SOCMAXdata1'
        batterymonitor_Convert_CurrMAXdata
          (&batterymonitor_DW.UnitDelay23_DSTATE[0],
           &batterymonitor_B.MAXdata_p[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion20'
        batterymonitor_DW.UnitDelay24_DSTATE[0] = rtb_MINdata_idx_1;
        batterymonitor_DW.UnitDelay24_DSTATE[1] = rtb_MINdata_h_idx_1;

        // MATLAB Function: '<S17>/Convert_SOCMINdata1'
        batterymonitor_Convert_CurrMAXdata
          (&batterymonitor_DW.UnitDelay24_DSTATE[0],
           &batterymonitor_B.MINdata_i[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion33'
        for (i = 0; i < 20; i++) {
          batterymonitor_DW.UnitDelay41_DSTATE[i] = rtb_RCData_l[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion33'

        // MATLAB Function: '<S17>/Convert_SOCRCdata'
        batterymonitor_Convert_CurrRCdata(&batterymonitor_DW.UnitDelay41_DSTATE
          [0], &batterymonitor_B.MAXdata_j[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion18'
        for (i = 0; i < 100; i++) {
          batterymonitor_DW.UnitDelay26_DSTATE[i] = rtb_Ydata_cq[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion18'

        // MATLAB Function: '<S17>/Convert_SOCYdata1'
        batterymonitor_Convert_CurrYdata(&batterymonitor_DW.UnitDelay26_DSTATE[0],
          &batterymonitor_B.Ydata_h[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion21' incorporates:
        //   MATLAB Function: '<S17>/Calculate_SOCInfo'
        //   UnitDelay: '<S17>/Unit Delay22'

        batterymonitor_DW.UnitDelay22_DSTATE = rtb_Temp_Derivative;

        // DataTypeConversion: '<S17>/Data Type Conversion22' incorporates:
        //   UnitDelay: '<S17>/Unit Delay25'

        batterymonitor_DW.UnitDelay25_DSTATE = rtb_MAXdata_f_idx_1;

        // MATLAB Function: '<S17>/MATLAB Function2' incorporates:
        //   Constant: '<S17>/MAXDerivative_SOC'
        //   Constant: '<S17>/MAXRange_SOC'
        //   Constant: '<S17>/MINDerivative_SOC'
        //   Constant: '<S17>/MINRange_SOC'
        //   DataTypeConversion: '<S17>/Data Type Conversion42'
        //   DataTypeConversion: '<S17>/Data Type Conversion43'
        //   UnitDelay: '<S17>/Unit Delay21'
        //   UnitDelay: '<S17>/Unit Delay44'

        batterymonitor_MATLABFunction(batterymonitor_DW.UnitDelay44_DSTATE,
          rtb_XaxisScale_i, -10.0, 110.0, rtb_RCValidCnt_k, 0.0, 500.0,
          static_cast<double>(rtb_ChargeStatus), static_cast<double>
          (batterymonitor_DW.UnitDelay21_DSTATE), &rtb_miss);

        // DataTypeConversion: '<S17>/Data Type Conversion24' incorporates:
        //   UnitDelay: '<S17>/Unit Delay44'

        rtb_y = std::fmod(std::floor(rtb_miss), 256.0);
        if (rtb_y < 0.0) {
          batterymonitor_DW.UnitDelay44_DSTATE = static_cast<uint8_t>(-
            static_cast<int8_t>(static_cast<uint8_t>(-rtb_y)));
        } else {
          batterymonitor_DW.UnitDelay44_DSTATE = static_cast<uint8_t>(rtb_y);
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion24'

        // DataTypeConversion: '<S17>/Data Type Conversion32' incorporates:
        //   UnitDelay: '<S17>/Unit Delay40'

        batterymonitor_DW.UnitDelay40_DSTATE = rtb_MINdata_l_idx_0;

        // SignalConversion generated from: '<S17>/SOCtRslt' incorporates:
        //   UnitDelay: '<S17>/Unit Delay44'

        batterymonitor_B.OutportBufferForSOCtRslt =
          batterymonitor_DW.UnitDelay44_DSTATE;

        // MATLAB Function: '<S17>/MATLAB Function7' incorporates:
        //   UnitDelay: '<S17>/Unit Delay34'
        //   UnitDelay: '<S17>/Unit Delay35'

        if (batterymonitor_B.RxTriggered.DataSerialNumber ==
            batterymonitor_DW.UnitDelay34_DSTATE) {
          rtb_y = 0.0;
        } else {
          rtb_y = (static_cast<double>
                   (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature) -
                   static_cast<double>(batterymonitor_DW.UnitDelay35_DSTATE)) /
            (static_cast<double>(batterymonitor_B.RxTriggered.DataSerialNumber)
             - static_cast<double>(batterymonitor_DW.UnitDelay34_DSTATE));
        }

        // End of MATLAB Function: '<S17>/MATLAB Function7'

        // Abs: '<S17>/Temp_Derivative'
        if (rtb_y < 0.0) {
          rtb_Temp_Derivative = static_cast<uint8_t>(std::fmod(std::ceil(-rtb_y),
            256.0));
        } else {
          rtb_Temp_Derivative = static_cast<uint8_t>(std::fmod(std::ceil(rtb_y),
            256.0));
        }

        // End of Abs: '<S17>/Temp_Derivative'

        // MATLAB Function: '<S17>/MATLAB Function11' incorporates:
        //   Constant: '<S17>/RCMAXDerivative_SOC1'
        //   Constant: '<S17>/RCMINDerivative_SOC1'

        batterymonitor_MATLABFunction10(rtb_Temp_Derivative, -10.0, 10.0,
          &rtb_miss);

        // MATLAB Function: '<S17>/Calculate_TempInfo' incorporates:
        //   MATLAB Function: '<S17>/Calculate_CurrInfo'
        //   UnitDelay: '<S17>/Unit Delay'
        //   UnitDelay: '<S17>/Unit Delay1'
        //   UnitDelay: '<S17>/Unit Delay2'
        //   UnitDelay: '<S17>/Unit Delay3'
        //   UnitDelay: '<S17>/Unit Delay4'
        //   UnitDelay: '<S17>/Unit Delay42'
        //   UnitDelay: '<S17>/Unit Delay43'
        //   UnitDelay: '<S17>/Unit Delay5'
        //   UnitDelay: '<S17>/Unit Delay6'

        std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
        for (i = 0; i < 20; i++) {
          rtb_RCData_l[i] = 0U;
        }

        if ((rtb_ChargeStatus != batterymonitor_DW.UnitDelay_DSTATE_g) &&
            (rtb_ChargeStatus == 1)) {
          rtb_count_a = 0U;
          rtb_RCValidCnt_k = 0U;
          rtb_Temp_Derivative = 0U;
          std::memset(&rtb_Ydata_cq[0], 0, 100U * sizeof(uint8_t));
          rtb_MAXdata_f_idx_1 = 0U;
          for (i = 0; i < 20; i++) {
            rtb_RCData_l[i] = 0U;
          }

          rtb_MAXdata_f_idx_0 = 0U;
          rtb_MINdata_l_idx_1 = UINT8_MAX;
          rtb_MINdata_l_idx_0 = 0U;
          rtb_MINdata_idx_1 = UINT8_MAX;
        } else if (rtb_ChargeStatus == 2) {
          rtb_y = std::round(batterymonitor_DW.UnitDelay6_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              rtb_count_a = static_cast<uint32_t>(rtb_y);
            } else {
              rtb_count_a = 0U;
            }
          } else {
            rtb_count_a = UINT32_MAX;
          }

          rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay4_DSTATE_m;
          rtb_Temp_Derivative = batterymonitor_DW.UnitDelay1_DSTATE_k;
          for (i = 0; i < 100; i++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay5_DSTATE[i]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_Ydata_cq[i] = 0U;
              }
            } else {
              rtb_Ydata_cq[i] = UINT8_MAX;
            }
          }

          rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay42_DSTATE;
          for (rtb_get = 0; rtb_get < 20; rtb_get++) {
            rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[rtb_get]);
            if (rtb_y < 256.0) {
              if (rtb_y >= 0.0) {
                rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
              } else {
                rtb_RCData_l[rtb_get] = 0U;
              }
            } else {
              rtb_RCData_l[rtb_get] = UINT8_MAX;
            }
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay2_DSTATE[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MAXdata_f_idx_0 = 0U;
            }
          } else {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE_h[0]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_1 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay2_DSTATE[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_l_idx_0 = 0U;
            }
          } else {
            rtb_MINdata_l_idx_0 = UINT8_MAX;
          }

          rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE_h[1]);
          if (rtb_y < 256.0) {
            if (rtb_y >= 0.0) {
              rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
            } else {
              rtb_MINdata_idx_1 = 0U;
            }
          } else {
            rtb_MINdata_idx_1 = UINT8_MAX;
          }
        } else {
          rtb_y = std::round(batterymonitor_DW.UnitDelay6_DSTATE);
          if (rtb_y < 4.294967296E+9) {
            if (rtb_y >= 0.0) {
              qY = static_cast<uint32_t>(rtb_y);
            } else {
              qY = 0U;
            }
          } else {
            qY = UINT32_MAX;
          }

          rtb_count_a = qY + /*MW:OvSatOk*/ 1U;
          if (qY + 1U < qY) {
            rtb_count_a = UINT32_MAX;
          }

          if (batterymonitor_DW.UnitDelay4_DSTATE_m > 7) {
            rtb_MAXdata_f_idx_0 = UINT8_MAX;
          } else {
            rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
              batterymonitor_DW.UnitDelay4_DSTATE_m);
          }

          if (mod_USSjf2FO(batterymonitor_DW.UnitDelay6_DSTATE,
                           rtb_MAXdata_f_idx_0) == 0) {
            if (batterymonitor_DW.UnitDelay1_DSTATE_k == 100) {
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay4_DSTATE_m + 1U);
              if (batterymonitor_DW.UnitDelay4_DSTATE_m + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_RCValidCnt_k = static_cast<uint8_t>(rtb_get);
              for (rtb_get = 0; rtb_get < 50; rtb_get++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay5_DSTATE[((rtb_get
                  + 1) << 1) - 2]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[rtb_get] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[rtb_get] = UINT8_MAX;
                }
              }

              rtb_Temp_Derivative = 51U;
              qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
              if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature > 255U)
              {
                qY = 255U;
              }

              rtb_Ydata_cq[50] = static_cast<uint8_t>(qY);
            } else {
              rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay4_DSTATE_m;
              rtb_get = static_cast<int32_t>
                (batterymonitor_DW.UnitDelay1_DSTATE_k + 1U);
              if (batterymonitor_DW.UnitDelay1_DSTATE_k + 1U > 255U) {
                rtb_get = 255;
              }

              rtb_Temp_Derivative = static_cast<uint8_t>(rtb_get);
              for (i = 0; i < 100; i++) {
                rtb_y = std::round(batterymonitor_DW.UnitDelay5_DSTATE[i]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_Ydata_cq[i] = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_Ydata_cq[i] = 0U;
                  }
                } else {
                  rtb_Ydata_cq[i] = UINT8_MAX;
                }
              }

              qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
              if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature > 255U)
              {
                qY = 255U;
              }

              rtb_Ydata_cq[static_cast<uint8_t>(rtb_get) - 1] =
                static_cast<uint8_t>(qY);
            }
          } else {
            rtb_RCValidCnt_k = batterymonitor_DW.UnitDelay4_DSTATE_m;
            rtb_Temp_Derivative = batterymonitor_DW.UnitDelay1_DSTATE_k;
            for (rtb_get = 0; rtb_get < 100; rtb_get++) {
              rtb_y = std::round(batterymonitor_DW.UnitDelay5_DSTATE[rtb_get]);
              if (rtb_y < 256.0) {
                if (rtb_y >= 0.0) {
                  rtb_Ydata_cq[rtb_get] = static_cast<uint8_t>(rtb_y);
                } else {
                  rtb_Ydata_cq[rtb_get] = 0U;
                }
              } else {
                rtb_Ydata_cq[rtb_get] = UINT8_MAX;
              }
            }
          }

          if (batterymonitor_DW.UnitDelay6_DSTATE == 0.0) {
            rtb_count_a = 1U;
            rtb_MAXdata_f_idx_1 = 0U;
            for (i = 0; i < 20; i++) {
              rtb_RCData_l[i] = 0U;
            }

            rtb_MAXdata_f_idx_0 = 0U;
            qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
            if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature > 255U) {
              qY = 255U;
            }

            rtb_MINdata_l_idx_0 = static_cast<uint8_t>(qY);
            rtb_MINdata_l_idx_1 = 0U;
            qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
            if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature > 255U) {
              qY = 255U;
            }

            rtb_MINdata_idx_1 = static_cast<uint8_t>(qY);
          } else {
            if (batterymonitor_DW.UnitDelay4_DSTATE_m > 7) {
              rtb_MAXdata_f_idx_0 = UINT8_MAX;
            } else {
              rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(1 <<
                batterymonitor_DW.UnitDelay4_DSTATE_m);
            }

            if ((mod_USSjf2FO(batterymonitor_DW.UnitDelay6_DSTATE,
                              rtb_MAXdata_f_idx_0) == 0) &&
                (batterymonitor_DW.UnitDelay1_DSTATE_k == 100)) {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay42_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay42_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay42_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay43_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }

                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_RCData_l[rtb_get - 1] = 50U;
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
                  if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                      255U) {
                    qY = 255U;
                  }

                  rtb_RCData_l[rtb_get - 1] = static_cast<uint8_t>(qY);
                } else {
                  rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay42_DSTATE;
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  for (i = 0; i < 10; i++) {
                    rtb_get = ((i + 1) << 1) - 2;
                    rtb_y = std::round
                      (batterymonitor_DW.UnitDelay43_DSTATE[rtb_get] / 2.0);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[rtb_get] = 0U;
                      }
                    } else {
                      rtb_RCData_l[rtb_get] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay42_DSTATE;
                if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }

                for (i = 0; i < 10; i++) {
                  rtb_get = ((i + 1) << 1) - 2;
                  rtb_y = std::round
                    (batterymonitor_DW.UnitDelay43_DSTATE[rtb_get] / 2.0);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[rtb_get] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[rtb_get] = 0U;
                    }
                  } else {
                    rtb_RCData_l[rtb_get] = UINT8_MAX;
                  }
                }
              }

              if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >=
                  batterymonitor_DW.UnitDelay2_DSTATE[1]) {
                rtb_MAXdata_f_idx_0 = 50U;
                qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
                if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                    255U) {
                  qY = 255U;
                }

                rtb_MINdata_l_idx_0 = static_cast<uint8_t>(qY);
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay2_DSTATE[0] / 2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay2_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_0 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_0 = UINT8_MAX;
                }
              }

              if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature <=
                  batterymonitor_DW.UnitDelay3_DSTATE_h[1]) {
                rtb_MINdata_l_idx_1 = 50U;
                qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
                if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                    255U) {
                  qY = 255U;
                }

                rtb_MINdata_idx_1 = static_cast<uint8_t>(qY);
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE_h[0] /
                                   2.0);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE_h[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_idx_1 = UINT8_MAX;
                }
              }
            } else {
              if (rtb_miss != 0.0) {
                if (batterymonitor_DW.UnitDelay42_DSTATE < 10) {
                  rtb_get = static_cast<int32_t>
                    (batterymonitor_DW.UnitDelay42_DSTATE + 1U);
                  if (batterymonitor_DW.UnitDelay42_DSTATE + 1U > 255U) {
                    rtb_get = 255;
                  }

                  rtb_MAXdata_f_idx_1 = static_cast<uint8_t>(rtb_get);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_get = 255;
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 1U);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 1U > 255U) {
                    rtb_get = 255;
                  }

                  qY = rtb_Temp_Derivative -
                    /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                    /*MW:OvSatOk*/ 1U;
                  if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                    qY = 0U;
                  }

                  rtb_RCData_l[rtb_get - 1] = static_cast<uint8_t>(qY);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  rtb_get = static_cast<int32_t>(rtb_MAXdata_f_idx_0 + 2U);
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_MAXdata_f_idx_0 = UINT8_MAX;
                  } else {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  if (rtb_MAXdata_f_idx_0 + 2U > 255U) {
                    rtb_get = 255;
                  }

                  qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
                  if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                      255U) {
                    qY = 255U;
                  }

                  rtb_RCData_l[rtb_get - 1] = static_cast<uint8_t>(qY);
                } else {
                  rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay42_DSTATE;
                  if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                    rtb_get = 255;
                  } else {
                    rtb_get = static_cast<uint8_t>
                      (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                  }

                  for (i = 0; i < rtb_get; i++) {
                    rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[i]);
                    if (rtb_y < 256.0) {
                      if (rtb_y >= 0.0) {
                        rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                      } else {
                        rtb_RCData_l[i] = 0U;
                      }
                    } else {
                      rtb_RCData_l[i] = UINT8_MAX;
                    }
                  }
                }
              } else {
                rtb_MAXdata_f_idx_1 = batterymonitor_DW.UnitDelay42_DSTATE;
                if (batterymonitor_DW.UnitDelay42_DSTATE > 127) {
                  rtb_get = 255;
                } else {
                  rtb_get = static_cast<uint8_t>
                    (batterymonitor_DW.UnitDelay42_DSTATE << 1);
                }

                for (i = 0; i < rtb_get; i++) {
                  rtb_y = std::round(batterymonitor_DW.UnitDelay43_DSTATE[i]);
                  if (rtb_y < 256.0) {
                    if (rtb_y >= 0.0) {
                      rtb_RCData_l[i] = static_cast<uint8_t>(rtb_y);
                    } else {
                      rtb_RCData_l[i] = 0U;
                    }
                  } else {
                    rtb_RCData_l[i] = UINT8_MAX;
                  }
                }
              }

              if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >=
                  batterymonitor_DW.UnitDelay2_DSTATE[1]) {
                qY = rtb_Temp_Derivative - 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(qY);
                qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
                if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                    255U) {
                  qY = 255U;
                }

                rtb_MINdata_l_idx_0 = static_cast<uint8_t>(qY);
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay2_DSTATE[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MAXdata_f_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MAXdata_f_idx_0 = 0U;
                  }
                } else {
                  rtb_MAXdata_f_idx_0 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay2_DSTATE[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_0 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_0 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_0 = UINT8_MAX;
                }
              }

              if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature <=
                  batterymonitor_DW.UnitDelay3_DSTATE_h[1]) {
                qY = rtb_Temp_Derivative -
                  /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
                  /*MW:OvSatOk*/ 1U;
                if (rtb_Temp_Derivative - 1U > rtb_Temp_Derivative) {
                  qY = 0U;
                }

                rtb_MINdata_l_idx_1 = static_cast<uint8_t>(qY);
                qY = batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;
                if (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                    255U) {
                  qY = 255U;
                }

                rtb_MINdata_idx_1 = static_cast<uint8_t>(qY);
              } else {
                rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE_h[0]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_l_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_l_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_l_idx_1 = UINT8_MAX;
                }

                rtb_y = std::round(batterymonitor_DW.UnitDelay3_DSTATE_h[1]);
                if (rtb_y < 256.0) {
                  if (rtb_y >= 0.0) {
                    rtb_MINdata_idx_1 = static_cast<uint8_t>(rtb_y);
                  } else {
                    rtb_MINdata_idx_1 = 0U;
                  }
                } else {
                  rtb_MINdata_idx_1 = UINT8_MAX;
                }
              }
            }
          }
        }

        // DataTypeConversion: '<S17>/Data Type Conversion1'
        batterymonitor_DW.UnitDelay2_DSTATE[0] = rtb_MAXdata_f_idx_0;
        batterymonitor_DW.UnitDelay2_DSTATE[1] = rtb_MINdata_l_idx_0;

        // MATLAB Function: '<S17>/Convert_TempMAXdata'
        batterymonitor_Convert_CurrMAXdata(&batterymonitor_DW.UnitDelay2_DSTATE
          [0], &batterymonitor_B.MAXdata_i[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion2'
        batterymonitor_DW.UnitDelay3_DSTATE_h[0] = rtb_MINdata_l_idx_1;
        batterymonitor_DW.UnitDelay3_DSTATE_h[1] = rtb_MINdata_idx_1;

        // MATLAB Function: '<S17>/Convert_TempMINdata'
        batterymonitor_Convert_CurrMAXdata
          (&batterymonitor_DW.UnitDelay3_DSTATE_h[0],
           &batterymonitor_B.MINdata_g[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion35'
        for (i = 0; i < 20; i++) {
          batterymonitor_DW.UnitDelay43_DSTATE[i] = rtb_RCData_l[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion35'

        // MATLAB Function: '<S17>/Convert_TempRCdata'
        batterymonitor_Convert_CurrRCdata(&batterymonitor_DW.UnitDelay43_DSTATE
          [0], &batterymonitor_B.MAXdata_k[0]);

        // DataTypeConversion: '<S17>/Data Type Conversion'
        for (i = 0; i < 100; i++) {
          batterymonitor_DW.UnitDelay5_DSTATE[i] = rtb_Ydata_cq[i];
        }

        // End of DataTypeConversion: '<S17>/Data Type Conversion'

        // MATLAB Function: '<S17>/Convert_TempYdata'
        batterymonitor_Convert_CurrYdata(&batterymonitor_DW.UnitDelay5_DSTATE[0],
          &batterymonitor_B.Ydata_j[0]);

        // MATLAB Function: '<S17>/MATLAB Function3' incorporates:
        //   Constant: '<S17>/MAXRange_Temp'
        //   Constant: '<S17>/MINRange_Temp'
        //   DataTypeConversion: '<S17>/Data Type Conversion40'
        //   UnitDelay: '<S17>/Unit Delay'
        //   UnitDelay: '<S17>/Unit Delay45'

        rtb_get = 0;
        if ((rtb_ChargeStatus == batterymonitor_DW.UnitDelay_DSTATE_g) ||
            (rtb_ChargeStatus != 1)) {
          if (batterymonitor_DW.UnitDelay45_DSTATE == 1) {
            rtb_get = 1;
          } else if ((batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature <
                      60U) ||
                     (batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature >
                      80U)) {
            rtb_get = 1;
          }
        }

        // End of MATLAB Function: '<S17>/MATLAB Function3'

        // DataTypeConversion: '<S17>/Data Type Conversion25' incorporates:
        //   UnitDelay: '<S17>/Unit Delay45'

        batterymonitor_DW.UnitDelay45_DSTATE = static_cast<uint8_t>(rtb_get);

        // DataTypeConversion: '<S17>/Data Type Conversion3' incorporates:
        //   MATLAB Function: '<S17>/Calculate_TempInfo'
        //   UnitDelay: '<S17>/Unit Delay1'

        batterymonitor_DW.UnitDelay1_DSTATE_k = rtb_Temp_Derivative;

        // DataTypeConversion: '<S17>/Data Type Conversion34' incorporates:
        //   UnitDelay: '<S17>/Unit Delay42'

        batterymonitor_DW.UnitDelay42_DSTATE = rtb_MAXdata_f_idx_1;

        // DataTypeConversion: '<S17>/Data Type Conversion4' incorporates:
        //   UnitDelay: '<S17>/Unit Delay4'

        batterymonitor_DW.UnitDelay4_DSTATE_m = rtb_RCValidCnt_k;

        // SignalConversion generated from: '<S17>/TempRslt' incorporates:
        //   UnitDelay: '<S17>/Unit Delay45'

        batterymonitor_B.OutportBufferForTempRslt =
          batterymonitor_DW.UnitDelay45_DSTATE;

        // Update for UnitDelay: '<S17>/Unit Delay7'
        batterymonitor_DW.UnitDelay7_DSTATE = rtb_ChargeStatus;

        // Update for UnitDelay: '<S17>/Unit Delay28'
        batterymonitor_DW.UnitDelay28_DSTATE =
          batterymonitor_B.RxTriggered.DataSerialNumber;

        // Update for UnitDelay: '<S17>/Unit Delay29'
        batterymonitor_DW.UnitDelay29_DSTATE = rtb_Abs;

        // Update for UnitDelay: '<S17>/Unit Delay13' incorporates:
        //   DataTypeConversion: '<S17>/Data Type Conversion11'

        batterymonitor_DW.UnitDelay13_DSTATE = rtb_count_h;

        // Update for UnitDelay: '<S17>/Unit Delay14'
        batterymonitor_DW.UnitDelay14_DSTATE = rtb_ChargeStatus;

        // Update for UnitDelay: '<S17>/Unit Delay30'
        batterymonitor_DW.UnitDelay30_DSTATE =
          batterymonitor_B.RxTriggered.DataSerialNumber;

        // Update for UnitDelay: '<S17>/Unit Delay31'
        batterymonitor_DW.UnitDelay31_DSTATE = rtb_Curr_Derivative;

        // Update for UnitDelay: '<S17>/Unit Delay20' incorporates:
        //   DataTypeConversion: '<S17>/Data Type Conversion17'

        batterymonitor_DW.UnitDelay20_DSTATE = rtb_count;

        // Update for UnitDelay: '<S17>/Unit Delay21'
        batterymonitor_DW.UnitDelay21_DSTATE = rtb_ChargeStatus;

        // Update for UnitDelay: '<S17>/Unit Delay32'
        batterymonitor_DW.UnitDelay32_DSTATE =
          batterymonitor_B.RxTriggered.DataSerialNumber;

        // Update for UnitDelay: '<S17>/Unit Delay33'
        batterymonitor_DW.UnitDelay33_DSTATE = rtb_XaxisScale_i;

        // Update for UnitDelay: '<S17>/Unit Delay27' incorporates:
        //   DataTypeConversion: '<S17>/Data Type Conversion23'

        batterymonitor_DW.UnitDelay27_DSTATE = rtb_count_cl;

        // Update for UnitDelay: '<S17>/Unit Delay'
        batterymonitor_DW.UnitDelay_DSTATE_g = rtb_ChargeStatus;

        // Update for UnitDelay: '<S17>/Unit Delay34'
        batterymonitor_DW.UnitDelay34_DSTATE =
          batterymonitor_B.RxTriggered.DataSerialNumber;

        // Update for UnitDelay: '<S17>/Unit Delay35'
        batterymonitor_DW.UnitDelay35_DSTATE =
          batterymonitor_B.RxTriggered.EMS_BatteryAvgTemperature;

        // Update for UnitDelay: '<S17>/Unit Delay6' incorporates:
        //   DataTypeConversion: '<S17>/Data Type Conversion5'

        batterymonitor_DW.UnitDelay6_DSTATE = rtb_count_a;

        // End of Outputs for SubSystem: '<S2>/Subsystem1'
      }

      // End of MATLAB Function: '<S2>/Calculate_GetCount'

      // MATLAB Function: '<S2>/Determine_ChargeResult' incorporates:
      //   UnitDelay: '<S2>/Unit Delay5'

      rtb_get = 0;
      if ((batterymonitor_B.OutportBufferForCurrenttRslt != 0) ||
          (batterymonitor_B.OutportBufferForVolttageRslt != 0) ||
          (batterymonitor_B.OutportBufferForSOCtRslt != 0) ||
          (batterymonitor_B.OutportBufferForTempRslt != 0) ||
          (batterymonitor_DW.UnitDelay5_DSTATE_k != 0U)) {
        rtb_get = 1;
      }

      // End of MATLAB Function: '<S2>/Determine_ChargeResult'

      // DataTypeConversion: '<S2>/Data Type Conversion2'
      rtb_DataTypeConversion2_g = static_cast<uint16_t>
        (batterymonitor_B.RxTriggered.EMS_HVBattSOC);

      // MATLAB Function: '<S2>/Calculate_CompleteTime'
      b_bu = batterymonitor_B.RxTriggered.EMS_RemainHour;
      rtb_count_h = batterymonitor_B.RxTriggered.EMS_RemainMin;
      rtb_count = 0U;
      rtb_count_cl = 0U;
      rtb_count_a = 0U;
      while (b_bu > 0U) {
        bu = rtb_count;
        rtb_miss = 16.0;
        qY = 1U;
        do {
          exitg1 = 0;
          if ((bu & 1U) != 0U) {
            rtb_y = rtb_miss * static_cast<double>(qY);
            if (rtb_y < 4.294967296E+9) {
              qY = static_cast<uint32_t>(rtb_y);
            } else {
              qY = UINT32_MAX;
            }
          }

          bu >>= 1U;
          if (bu == 0U) {
            exitg1 = 1;
          } else {
            rtb_miss *= rtb_miss;
          }
        } while (exitg1 == 0);

        bu = b_bu - b_bu / 10U * 10U;
        qY = mul_u32_sat(bu, qY) + /*MW:OvSatOk*/ rtb_count_cl;
        if (qY < rtb_count_cl) {
          qY = UINT32_MAX;
        }

        rtb_count_cl = qY;
        qY = rtb_count + /*MW:OvSatOk*/ 1U;
        if (rtb_count + 1U < rtb_count) {
          qY = UINT32_MAX;
        }

        rtb_count = qY;
        if (bu < 5U) {
          b_bu = static_cast<uint32_t>(std::round(static_cast<double>(b_bu) /
            10.0));
        } else {
          qY = static_cast<uint32_t>(std::round(static_cast<double>(b_bu) / 10.0));
          b_bu = qY -
            /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
            /*MW:OvSatOk*/ 1U;
          if (qY - 1U > qY) {
            b_bu = 0U;
          }
        }
      }

      rtb_count = 0U;
      while (rtb_count_h > 0U) {
        b_bu = rtb_count;
        rtb_miss = 16.0;
        qY = 1U;
        do {
          exitg1 = 0;
          if ((b_bu & 1U) != 0U) {
            rtb_y = rtb_miss * static_cast<double>(qY);
            if (rtb_y < 4.294967296E+9) {
              qY = static_cast<uint32_t>(rtb_y);
            } else {
              qY = UINT32_MAX;
            }
          }

          b_bu >>= 1U;
          if (b_bu == 0U) {
            exitg1 = 1;
          } else {
            rtb_miss *= rtb_miss;
          }
        } while (exitg1 == 0);

        bu = rtb_count_h - rtb_count_h / 10U * 10U;
        qY = mul_u32_sat(bu, qY) + /*MW:OvSatOk*/ rtb_count_a;
        if (qY < rtb_count_a) {
          qY = UINT32_MAX;
        }

        rtb_count_a = qY;
        qY = rtb_count + /*MW:OvSatOk*/ 1U;
        if (rtb_count + 1U < rtb_count) {
          qY = UINT32_MAX;
        }

        rtb_count = qY;
        if (bu < 5U) {
          rtb_count_h = static_cast<uint32_t>(std::round(static_cast<double>
            (rtb_count_h) / 10.0));
        } else {
          qY = static_cast<uint32_t>(std::round(static_cast<double>(rtb_count_h)
            / 10.0));
          rtb_count_h = qY -
            /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
            /*MW:OvSatOk*/ 1U;
          if (qY - 1U > qY) {
            rtb_count_h = 0U;
          }
        }
      }

      if (rtb_count_cl > 255U) {
        rtb_count_cl = 255U;
      }

      expl_temp.CompleteTime[0] = static_cast<uint8_t>(rtb_count_cl);
      if (rtb_count_a > 255U) {
        rtb_count_a = 255U;
      }

      expl_temp.CompleteTime[1] = static_cast<uint8_t>(rtb_count_a);

      // End of MATLAB Function: '<S2>/Calculate_CompleteTime'

      // MATLAB Function: '<S2>/Calculate_PresentTime'
      for (i = 0; i < 8; i++) {
        expl_temp.Date[i] = 0U;
      }

      //시간계산 시작
      now = std::time(nullptr);      
      now += 9 * 60 * 60;                   
      tm* kstTime = std::gmtime(&now);      

      expl_temp.Date[0] = (kstTime->tm_year + 1900) / 100;  
      expl_temp.Date[1] = (kstTime->tm_year + 1900) % 100;  
      expl_temp.Date[2] = kstTime->tm_mon + 1;              
      expl_temp.Date[3] = kstTime->tm_mday;                 
      expl_temp.Date[4] = kstTime->tm_hour;                 
      expl_temp.Date[5] = kstTime->tm_min;                  
      expl_temp.Date[6] = kstTime->tm_sec;

      for(int i=0;i<7;i++)
        expl_temp.Date[i] = toBCD(expl_temp.Date[i]);
      //시작계산 끝

      // End of MATLAB Function: '<S2>/Calculate_PresentTime'

      // MATLAB Function: '<S2>/Calculate_FinishedTime'
      for (i = 0; i < 7; i++) {
        expl_temp.FinishedTime[i] = 0U;
      }

      //시간계산 시작
      int additionalSeconds = batterymonitor_B.RxTriggered.EMS_RemainHour * 60 * 60 + batterymonitor_B.RxTriggered.EMS_RemainMin * 60;
      finishtime = now + additionalSeconds;
      tm* kstFinishTime = std::gmtime(&finishtime);

      expl_temp.FinishedTime[0] = (kstFinishTime->tm_year + 1900) / 100;
      expl_temp.FinishedTime[1] = (kstFinishTime->tm_year + 1900) % 100;
      expl_temp.FinishedTime[2] = kstFinishTime->tm_mon + 1;
      expl_temp.FinishedTime[3] = kstFinishTime->tm_mday;
      expl_temp.FinishedTime[4] = kstFinishTime->tm_hour;
      expl_temp.FinishedTime[5] = kstFinishTime->tm_min;
      expl_temp.FinishedTime[6] = kstFinishTime->tm_sec;

      for(int i=0;i<7;i++)
        expl_temp.FinishedTime[i] = toBCD(expl_temp.FinishedTime[i]);
      //시작계산 끝

      // End of MATLAB Function: '<S2>/Calculate_FinishedTime'

      // Gain: '<S2>/Gain1'
      qY = 3435973837U;
      uMultiWordMul(&qY, 1, &batterymonitor_B.RxTriggered.EMS_HVBattSOE, 1,
                    &rtb_Gain1.chunks[0U], 2);

      // MATLAB Function: '<S2>/Calculate_DTE'
      if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp.chunks[0U], 2)) {
        i = 11;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_a.chunks[0U], 2)) {
        i = 10;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_9.chunks[0U], 2)) {
        i = 9;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_8.chunks[0U], 2)) {
        i = 8;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_7.chunks[0U], 2)) {
        i = 7;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_6.chunks[0U], 2)) {
        i = 6;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_5.chunks[0U], 2)) {
        i = 5;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_4.chunks[0U], 2)) {
        i = 4;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_3.chunks[0U], 2)) {
        i = 3;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_2.chunks[0U], 2)) {
        i = 2;
      } else {
        i = uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp_1.chunks[0U], 2);
      }

      if (uMultiWordLe(&rtb_Gain1.chunks[0U], &tmp_0.chunks[0U], 2)) {
        rtb_y = 0.0;
      } else if (uMultiWordGe(&rtb_Gain1.chunks[0U], &tmp.chunks[0U], 2)) {
        rtb_y = 430.0;
      } else {
        rtb_y = (uMultiWord2Double(&rtb_Gain1.chunks[0U], 2, 0) *
                 2.9103830456733704E-11 - static_cast<double>(c[i])) /
          static_cast<double>(c[i + 1] - c[i]) * (b[i + 1] - b[i]) + b[i];
      }

      // DataTypeConversion: '<S2>/Data Type Conversion3'
      rtb_DataTypeConversion3 = static_cast<uint16_t>
        (batterymonitor_B.RxTriggered.EMS_HVBattSOE);

      // Logic: '<S1>/Logical Operator'
      rtb_FixPtRelationalOperator = (rtb_FixPtRelationalOperator ||
        rtb_FixPtRelationalOperator_k);

      // Outputs for Triggered SubSystem: '<S1>/SendMessage' incorporates:
      //   TriggerPort: '<S4>/Trigger'

      if (rtb_FixPtRelationalOperator &&
          (batterymonitor_PrevZCX.SendMessage_Trig_ZCE != POS_ZCSIG)) {
        // BusCreator: '<S4>/Bus Creator' incorporates:
        //   DataTypeConversion: '<S2>/Data Type Conversion11'
        //   DataTypeConversion: '<S2>/Data Type Conversion2'
        //   DataTypeConversion: '<S2>/Data Type Conversion3'
        //   MATLAB Function: '<S4>/MATLAB Function12'
        //   Sum: '<S4>/Subtract'
        //   Sum: '<S4>/Subtract1'
        //   UnitDelay: '<S17>/Unit Delay1'
        //   UnitDelay: '<S17>/Unit Delay11'
        //   UnitDelay: '<S17>/Unit Delay15'
        //   UnitDelay: '<S17>/Unit Delay18'
        //   UnitDelay: '<S17>/Unit Delay22'
        //   UnitDelay: '<S17>/Unit Delay25'
        //   UnitDelay: '<S17>/Unit Delay36'
        //   UnitDelay: '<S17>/Unit Delay38'
        //   UnitDelay: '<S17>/Unit Delay4'
        //   UnitDelay: '<S17>/Unit Delay40'
        //   UnitDelay: '<S17>/Unit Delay42'
        //   UnitDelay: '<S17>/Unit Delay44'
        //   UnitDelay: '<S17>/Unit Delay45'
        //   UnitDelay: '<S17>/Unit Delay46'
        //   UnitDelay: '<S17>/Unit Delay47'
        //   UnitDelay: '<S17>/Unit Delay8'
        //   UnitDelay: '<S2>/Unit Delay5'
        //   UnitDelay: '<S4>/Unit Delay1'
        //   UnitDelay: '<S4>/Unit Delay2'

        // Send event
        expl_temp.ChargeStopReason = batterymonitor_DW.UnitDelay5_DSTATE_k;
        //시간계산 시작
        elapsedtime = now - starttime;
        starttime = now;
        expl_temp.ElapsedTime[0] = elapsedtime / 3600;
        expl_temp.ElapsedTime[1] = elapsedtime / 60;
        for(int i=0;i<2;i++)
          expl_temp.ElapsedTime[i] = toBCD(expl_temp.ElapsedTime[i]);
        //시간계산 끝
        expl_temp.Temp.tRslt = batterymonitor_DW.UnitDelay45_DSTATE;
        expl_temp.Temp.XaxisScale = batterymonitor_DW.UnitDelay4_DSTATE_m;
        expl_temp.Temp.YaxisValidCnt = batterymonitor_DW.UnitDelay1_DSTATE_k;
        std::memcpy(&expl_temp.Temp.Ydata[0], &batterymonitor_B.Ydata_j[0], 100U
                    * sizeof(uint8_t));
        expl_temp.Temp.RCValidCnt = batterymonitor_DW.UnitDelay42_DSTATE;
        for (i = 0; i < 20; i++) {
          expl_temp.Temp.RCdata[i] = batterymonitor_B.MAXdata_k[i];
        }

        expl_temp.Temp.MAXdata[0] = batterymonitor_B.MAXdata_i[0];
        expl_temp.Temp.MINdata[0] = batterymonitor_B.MINdata_g[0];
        expl_temp.Temp.MAXdata[1] = batterymonitor_B.MAXdata_i[1];
        expl_temp.Temp.MINdata[1] = batterymonitor_B.MINdata_g[1];
        expl_temp.SOC.tRslt = batterymonitor_DW.UnitDelay44_DSTATE;
        expl_temp.SOC.XaxisScale = batterymonitor_DW.UnitDelay25_DSTATE;
        expl_temp.SOC.YaxisValidCnt = batterymonitor_DW.UnitDelay22_DSTATE;
        std::memcpy(&expl_temp.SOC.Ydata[0], &batterymonitor_B.Ydata_h[0], 100U *
                    sizeof(uint8_t));
        expl_temp.SOC.RCValidCnt = batterymonitor_DW.UnitDelay40_DSTATE;
        for (i = 0; i < 20; i++) {
          expl_temp.SOC.RCdata[i] = batterymonitor_B.MAXdata_j[i];
        }

        expl_temp.SOC.MAXdata[0] = batterymonitor_B.MAXdata_p[0];
        expl_temp.SOC.MINdata[0] = batterymonitor_B.MINdata_i[0];
        expl_temp.SOC.MAXdata[1] = batterymonitor_B.MAXdata_p[1];
        expl_temp.SOC.MINdata[1] = batterymonitor_B.MINdata_i[1];
        expl_temp.Voltage.tRslt = batterymonitor_DW.UnitDelay47_DSTATE;
        expl_temp.Voltage.XaxisScale = batterymonitor_DW.UnitDelay18_DSTATE;
        expl_temp.Voltage.YaxisValidCnt = batterymonitor_DW.UnitDelay15_DSTATE;
        std::memcpy(&expl_temp.Voltage.Ydata[0], &batterymonitor_B.Ydata[0],
                    100U * sizeof(uint8_t));
        expl_temp.Voltage.RCValidCnt = batterymonitor_DW.UnitDelay38_DSTATE;
        for (i = 0; i < 20; i++) {
          expl_temp.Voltage.RCdata[i] = batterymonitor_B.MAXdata[i];
        }

        expl_temp.Voltage.MAXdata[0] = batterymonitor_B.MAXdata_m[0];
        expl_temp.Voltage.MINdata[0] = batterymonitor_B.MINdata[0];
        expl_temp.Voltage.MAXdata[1] = batterymonitor_B.MAXdata_m[1];
        expl_temp.Voltage.MINdata[1] = batterymonitor_B.MINdata[1];
        expl_temp.Current.tRslt = batterymonitor_DW.UnitDelay46_DSTATE;
        expl_temp.Current.XaxisScale = batterymonitor_DW.UnitDelay11_DSTATE;
        expl_temp.Current.YaxisValidCnt = batterymonitor_DW.UnitDelay8_DSTATE;
        std::memcpy(&expl_temp.Current.Ydata[0], &batterymonitor_B.Ydata_p[0],
                    100U * sizeof(uint8_t));
        expl_temp.Current.RCValidCnt = batterymonitor_DW.UnitDelay36_DSTATE;
        for (i = 0; i < 20; i++) {
          expl_temp.Current.RCdata[i] = batterymonitor_B.MAXdata_b[i];
        }

        expl_temp.Current.MAXdata[0] = batterymonitor_B.MAXdata_o[0];
        expl_temp.Current.MINdata[0] = batterymonitor_B.MINdata_iq[0];
        expl_temp.Current.MAXdata[1] = batterymonitor_B.MAXdata_o[1];
        expl_temp.Current.MINdata[1] = batterymonitor_B.MINdata_iq[1];
        expl_temp.ChargedSOC = static_cast<uint16_t>(static_cast<uint16_t>
          (batterymonitor_B.RxTriggered.EMS_HVBattSOC) -
          batterymonitor_DW.UnitDelay2_DSTATE_k);
        expl_temp.ChargedSOE = static_cast<uint16_t>(static_cast<uint16_t>
          (batterymonitor_B.RxTriggered.EMS_HVBattSOE) -
          batterymonitor_DW.UnitDelay1_DSTATE_m);
        expl_temp.ChargeResult = static_cast<uint16_t>(rtb_get);
        expl_temp.MissedCount = rtb_y_b;
        expl_temp.GetCount = rtb_y_m;

        // MultiPortSwitch: '<S2>/Multiport Switch'
        if (batterymonitor_B.RxTriggered.EMS_ConnectionStatus == 1U) {
          // BusCreator: '<S4>/Bus Creator'
          expl_temp.TargetSOC = static_cast<uint8_t>
            (batterymonitor_B.RxTriggered.EMS_Avn_AcChargingTargetSetSts);
        } else {
          // BusCreator: '<S4>/Bus Creator'
          expl_temp.TargetSOC = static_cast<uint8_t>
            (batterymonitor_B.RxTriggered.EMS_Avn_DcChargingTargetSetSts);
        }

        // End of MultiPortSwitch: '<S2>/Multiport Switch'

        // BusCreator: '<S4>/Bus Creator' incorporates:
        //   Constant: '<S2>/Constant5'
        //   DataTypeConversion: '<S2>/Data Type Conversion3'
        //   DataTypeConversion: '<S2>/Data Type Conversion4'

        expl_temp.SOE = static_cast<uint16_t>
          (batterymonitor_B.RxTriggered.EMS_HVBattSOE);
        expl_temp.SOH = static_cast<uint16_t>
          (batterymonitor_B.RxTriggered.EMS_HVBattSOH);
        expl_temp.BatteryStatus = 0U;

        // MATLAB Function: '<S2>/Calculate_DTE'
        rtb_y = std::round(rtb_y);
        if (rtb_y < 65536.0) {
          if (rtb_y >= 0.0) {
            // BusCreator: '<S4>/Bus Creator'
            expl_temp.DTE = static_cast<uint16_t>(rtb_y);
          } else {
            // BusCreator: '<S4>/Bus Creator'
            expl_temp.DTE = 0U;
          }
        } else {
          // BusCreator: '<S4>/Bus Creator'
          expl_temp.DTE = UINT16_MAX;
        }

        // BusCreator: '<S4>/Bus Creator' incorporates:
        //   Constant: '<S2>/InterfaceIDforDbg_'
        //   DataTypeConversion: '<S2>/Data Type Conversion13'
        //   DataTypeConversion: '<S2>/Data Type Conversion2'
        //   UnitDelay: '<S2>/Unit Delay4'

        expl_temp.BattSOC = static_cast<uint16_t>
          (batterymonitor_B.RxTriggered.EMS_HVBattSOC);
        expl_temp.EMS_ConnectionStatus = static_cast<uint8_t>
          (batterymonitor_DW.UnitDelay4_DSTATE);
        expl_temp.ChargeStatus = rtb_ChargeStatus;
        expl_temp.VehicleUniqueSnr =
          batterymonitor_B.RxTriggered.VehicleUniqueSnr;
        expl_temp.InterfaceIDforDbg = 12U;
        // mLogger.LogInfo() << "MsgInfo 전송";
        P_MsgInfo->ems_MsgInfo.Send(expl_temp);

        // Update for UnitDelay: '<S4>/Unit Delay1'
        batterymonitor_DW.UnitDelay1_DSTATE_m = rtb_DataTypeConversion3;

        // Update for UnitDelay: '<S4>/Unit Delay2'
        batterymonitor_DW.UnitDelay2_DSTATE_k = rtb_DataTypeConversion2_g;
      }

      batterymonitor_PrevZCX.SendMessage_Trig_ZCE = rtb_FixPtRelationalOperator;

      // End of Outputs for SubSystem: '<S1>/SendMessage'

      // Update for UnitDelay: '<S51>/Delay Input1'
      //
      //  Block description for '<S51>/Delay Input1':
      //
      //   Store in Global RAM

      batterymonitor_DW.DelayInput1_DSTATE = rtb_start;

      // Update for UnitDelay: '<S50>/Delay Input1'
      //
      //  Block description for '<S50>/Delay Input1':
      //
      //   Store in Global RAM

      batterymonitor_DW.DelayInput1_DSTATE_n =
        batterymonitor_B.RxTriggered.EMS_HVBatt_MainRlyCloseReq;

      // Update for UnitDelay: '<S2>/Unit Delay3' incorporates:
      //   DataTypeConversion: '<S2>/Data Type Conversion8'

      batterymonitor_DW.UnitDelay3_DSTATE = rtb_ChargeStatus;

      // Update for UnitDelay: '<S2>/Unit Delay1'
      batterymonitor_DW.UnitDelay1_DSTATE = rtb_y_m;

      // Update for UnitDelay: '<S2>/Unit Delay'
      batterymonitor_DW.UnitDelay_DSTATE =
        batterymonitor_B.RxTriggered.DataSerialNumber;

      // Update for UnitDelay: '<S2>/Unit Delay2'
      batterymonitor_DW.UnitDelay2_DSTATE_a = rtb_y_b;

      // Update for UnitDelay: '<S2>/Unit Delay5'
      batterymonitor_DW.UnitDelay5_DSTATE_k =
        batterymonitor_B.RxTriggered.EMS_ReturnCode;

      // Update for UnitDelay: '<S2>/Unit Delay4'
      batterymonitor_DW.UnitDelay4_DSTATE =
        batterymonitor_B.RxTriggered.EMS_ConnectionStatus;

      // End of Outputs for SubSystem: '<Root>/BatteryMonitor_triggered_sys'
    }
  }
}

// Model initialize function
void batterymonitor::initialize_P_MsgInfo()
{
  {
    if(P_MsgInfo) return;
    // SystemInitialize for S-Function (sfun_private_function_caller) generated from: '<Root>/BatteryMonitor_triggered_sys' incorporates:
    //   SubSystem: '<Root>/BatteryMonitor_triggered_sys'

    // SystemInitialize for Triggered SubSystem: '<S1>/Subsystem'
    batterymonitor_PrevZCX.Subsystem_Trig_ZCE = UNINITIALIZED_ZCSIG;

    // End of SystemInitialize for SubSystem: '<S1>/Subsystem'

    // SystemInitialize for Triggered SubSystem: '<S1>/Subsystem1'
    batterymonitor_PrevZCX.Subsystem1_Trig_ZCE = UNINITIALIZED_ZCSIG;

    // End of SystemInitialize for SubSystem: '<S1>/Subsystem1'

    // SystemInitialize for Triggered SubSystem: '<S1>/SendMessage'
    batterymonitor_PrevZCX.SendMessage_Trig_ZCE = UNINITIALIZED_ZCSIG;

    // End of SystemInitialize for SubSystem: '<S1>/SendMessage'
    // End of SystemInitialize for S-Function (sfun_private_function_caller) generated from: '<Root>/BatteryMonitor_triggered_sys' 
    // Initialize service provider instance - P_MsgInfo
    P_MsgInfo = std::make_shared< batt::msginfo::skeleton::MsgInfoSrvSkeleton >
      (ara::core::InstanceSpecifier(ara::core::StringView(
         "BatteryMonitor/AA/P_MsgInfo")),
       ara::com::MethodCallProcessingMode::kEventSingleThread);
    P_MsgInfo->OfferService();
    mLogger.LogInfo() << "MsgInfo OfferService()";
  }
}

void batterymonitor::initialize_R_BmsInfo()
{
    if(R_BmsInfo) return;
    // ara::com::ServiceHandleContainer< battery::bmsinfo::proxy::BmsInfoSrvProxy::
    //   HandleType > handles;

    // std::shared_ptr<ara::core::Result<ara::com::ServiceHandleContainer< battery::
    //   bmsinfo::proxy::BmsInfoSrvProxy::HandleType >>> resultPtr;

    // // Initialize service requester instance - R_BmsInfo
    // resultPtr = std::make_shared< ara::core::Result<ara::com::
    //   ServiceHandleContainer< battery::bmsinfo::proxy::BmsInfoSrvProxy::
    //   HandleType >> >(battery::bmsinfo::proxy::BmsInfoSrvProxy::FindService(ara::
    //   core::InstanceSpecifier(ara::core::StringView(
    //   "BatteryMonitor/AA/R_BmsInfo"))));
    // if (resultPtr->HasValue()) {
    //   handles = resultPtr->Value();
    //   if (handles.size() > 0U) {
    //     R_BmsInfo = std::make_shared< battery::bmsinfo::proxy::BmsInfoSrvProxy >
    //       (*handles.begin());

    //     // Subscribe events
    //     R_BmsInfo->ems_BmsInfo.Subscribe(1U);
    //     R_BmsInfo->ems_BmsInfo.SetReceiveHandler(std::move(std::bind
    //       (&batterymonitor::BatteryMonitor_triggered_sys, this)));
    //   mLogger.LogInfo() << "BmsInfo Subscribed" ; 

    //   }
    // }


  ara::core::InstanceSpecifier specifier_msgInfo("BatteryMonitor/AA/R_BmsInfo");
  auto callback = [&](auto container, auto findHandle)
  {
    FindServiceCallback(container, findHandle);
  };

  std::unique_lock<std::mutex> lock(mHandle);

  auto result = battery::bmsinfo::proxy::BmsInfoSrvProxy::StartFindService(callback, specifier_msgInfo);

  if (cvHandle.wait_for(lock, std::chrono::milliseconds(2000)) == std::cv_status::timeout)
  {
    mLogger.LogInfo() << "cbhandle is empty";
    return;
  }

  if (!result.HasValue())
  {
    mLogger.LogInfo() << "bmsInfo StartFindService() Failed";
    return;
  }
  else
  {
    mLogger.LogInfo() << "bmsInfo StartFindService() Success";
  }    

}

void
batterymonitor::FindServiceCallback(
    ara::com::ServiceHandleContainer<battery::bmsinfo::proxy::BmsInfoSrvProxy::HandleType> container,
    ara::com::FindServiceHandle findHandle)
{
    mLogger.LogInfo() << __func__;


    std::lock_guard<std::mutex> lock(mHandle);

    int size = container.size();
    mLogger.LogInfo() << "bmsinfo container size = " << size;

    if (R_BmsInfo != nullptr)
    {
        UnsubscribeEvent();
        R_BmsInfo = nullptr;
    }

    if (container.empty())
    {
        mLogger.LogWarn() << __func__ << "container empty";
        R_BmsInfo = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    R_BmsInfo = std::make_shared<battery::bmsinfo::proxy::BmsInfoSrvProxy>(container.at(0));

    SubscribeEvent();

    cvHandle.notify_one();
    mLogger.LogInfo() << "bmsinfoService subscribe complete";
}

void
batterymonitor::SubscribeEvent()
{
    mLogger.LogInfo() << __func__;

    if (R_BmsInfo == nullptr)
    {
        return;
    }

    if (R_BmsInfo->ems_BmsInfo.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed)
    {
        return;
    }
    auto result = R_BmsInfo->ems_BmsInfo.SetReceiveHandler(std::bind(&batterymonitor::BatteryMonitor_triggered_sys, this));
    if (!result.HasValue())
    {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for ems_BmsInfo with " << result.Error().Message();
    }

    R_BmsInfo->ems_BmsInfo.Subscribe(10);
    if (!result.HasValue())
    {
        mLogger.LogWarn() << "Failed to Subscribe for ems_BmsInfo with " << result.Error().Message();
    }
}

void
batterymonitor::UnsubscribeEvent()
{
    if (R_BmsInfo == nullptr)
    {
        return;
    }

    R_BmsInfo->ems_BmsInfo.Unsubscribe();
}




void batterymonitor::initialize_R_LmpCtrl()
{
    if(R_LmpCtrl) return;
    // ara::com::ServiceHandleContainer< lmp::control::proxy::
    //   EevpControlSoaMImProxy::HandleType > handles_0;
    
    // std::shared_ptr<ara::core::Result<ara::com::ServiceHandleContainer< lmp::
    //   control::proxy::EevpControlSoaMImProxy::HandleType >>> resultPtr_0;

    // // Initialize service requester instance - R_LmpCtrl
    // resultPtr_0 = std::make_shared< ara::core::Result<ara::com::
    //   ServiceHandleContainer< lmp::control::proxy::EevpControlSoaMImProxy::
    //   HandleType >> >(lmp::control::proxy::EevpControlSoaMImProxy::FindService
    //                   (ara::core::InstanceSpecifier(ara::core::StringView(
    //   "BatteryMonitor/AA/R_LmpCtrl"))));
    // if (resultPtr_0->HasValue()) {
    //   handles_0 = resultPtr_0->Value();
    //   if (handles_0.size() > 0U) {
    //     R_LmpCtrl = std::make_shared< lmp::control::proxy::
    //       EevpControlSoaMImProxy >(*handles_0.begin());
    //   mLogger.LogInfo() << "LmpCtrl Subscribed" ; 
    //   }
    // }
ara::core::InstanceSpecifier specifier_LmpCtrl("BatteryMonitor/AA/R_LmpCtrl");
  auto callback = [&](auto container, auto findHandle)
  {
    FindLmpCallback(container, findHandle);
  };

  std::unique_lock<std::mutex> lock(mHandle);

  auto result = lmp::control::proxy::EevpControlSoaMImProxy::StartFindService(callback, specifier_LmpCtrl);

  if (cvHandle2.wait_for(lock, std::chrono::milliseconds(2000)) == std::cv_status::timeout)
  {
    mLogger.LogInfo() << "cbhandle is empty";
    return;
  }

  if (!result.HasValue())
  {
    mLogger.LogInfo() << "Lmpctrl StartFindService() Failed";
    return;
  }
  else
  {
    mLogger.LogInfo() << "Lmpctrl StartFindService() Success";
  }    

}

void
batterymonitor::FindLmpCallback(
        ara::com::ServiceHandleContainer<lmp::control::proxy::EevpControlSoaMImProxy::HandleType> container,
    ara::com::FindServiceHandle findHandle)
{
    mLogger.LogInfo() << __func__;


    std::lock_guard<std::mutex> lock(mHandle);

    int size = container.size();
    mLogger.LogInfo() << "Lmpctrl container size = " << size;

    if (R_LmpCtrl != nullptr)
    {
        R_LmpCtrl = nullptr;
    }

    if (container.empty())
    {
        mLogger.LogWarn() << __func__ << "container empty";
        R_LmpCtrl = nullptr;
        return;
    }

    nFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    R_LmpCtrl = std::make_shared<lmp::control::proxy::EevpControlSoaMImProxy>(container.at(0));

    cvHandle2.notify_one();
    mLogger.LogInfo() << "Lmpctrl subscribe complete";
}




// Model terminate function
void batterymonitor::terminate()
{
  P_MsgInfo->StopOfferService();
}

// Constructor
batterymonitor::batterymonitor():
  batterymonitor_B(),
  batterymonitor_DW(),
  batterymonitor_PrevZCX(),
  mLogger(ara::log::CreateLogger("BATT", "BATT", ara::log::LogLevel::kInfo))

{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
batterymonitor::~batterymonitor() = default;
