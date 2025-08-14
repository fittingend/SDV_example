//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: Control.cpp
//
// Code generated for Simulink model 'Control'.
//
// Model version                  : 1.7
// Simulink Coder version         : 24.1 (R2024a) 19-Nov-2023
// C/C++ source code generated on : Thu Nov 14 17:25:33 2024
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Control.h"
#include "rtwtypes.h"
#include "Control_private.h"

// External inputs (root inport signals with default storage)
Control_ExtU_T Control_U;

// External outputs (root outports fed by signals with default storage)
Control_ExtY_T Control_Y;

// Real-time model
Control_RT_MODEL_T Control_M_{ };

Control_RT_MODEL_T *const Control_M{ &Control_M_ };

// Model step function
void Control_step(void)
{
  boolean_T rtb_Compare;
  boolean_T rtb_or_logic;

  // Logic: '<Root>/or_logic2' incorporates:
  //   Constant: '<S1>/Constant'
  //   Inport: '<Root>/Fwd_Sensor'
  //   RelationalOperator: '<S1>/Compare'

  Control_Y.output_Accel = (Control_U.Fwd_Sensor <= 3.0);

  // RelationalOperator: '<S2>/Compare' incorporates:
  //   Constant: '<S2>/Constant'
  //   Inport: '<Root>/Rear_Sensor'

  rtb_Compare = (Control_U.Rear_Sensor <= 3.0);

  // Logic: '<Root>/or_logic'
  rtb_or_logic = (Control_Y.output_Accel || rtb_Compare);

  // Outport: '<Root>/output_Beep_Large'
  Control_Y.output_Beep_Large = rtb_or_logic;

  // RelationalOperator: '<S4>/Compare' incorporates:
  //   Constant: '<S3>/Constant'
  //   Inport: '<Root>/Fwd_Sensor'
  //   Inport: '<Root>/Rear_Sensor'
  //   Logic: '<Root>/and_logic_Front'
  //   Logic: '<Root>/and_logic_Rear'
  //   RelationalOperator: '<S3>/Compare'
  //   Switch: '<Root>/3m_com'

  Control_Y.output_Beep = ((Control_U.Fwd_Sensor <= 5.0) &&
    (Control_U.Rear_Sensor >= 3.0));

  // Switch: '<Root>/Bk_move'
  if (Control_Y.output_Beep) {
    // Switch: '<Root>/Bk_move'
    Control_Y.output_Brake = 1.0;
  } else {
    // Switch: '<Root>/Bk_move' incorporates:
    //   Constant: '<Root>/bk_no_move'

    Control_Y.output_Brake = 0.0;
  }

  // End of Switch: '<Root>/Bk_move'

  // Switch: '<Root>/light_Beep_com'
  Control_Y.output_Light = Control_Y.output_Beep;

  // RelationalOperator: '<S4>/Compare' incorporates:
  //   Constant: '<S4>/Constant'
  //   Inport: '<Root>/Fwd_Sensor'

  Control_Y.output_Beep = (Control_U.Fwd_Sensor <= 7.0);

  // Logic: '<Root>/or_logic2' incorporates:
  //   Logic: '<Root>/or_logic1'

  Control_Y.output_Accel = (Control_Y.output_Accel && (!rtb_Compare));

  // Matfile logging
  rt_UpdateTXYLogVars(Control_M->rtwLogInfo, (&Control_M->Timing.taskTime0));

  // signal main to stop simulation
  {                                    // Sample time: [0.3s, 0.0s]
    if ((rtmGetTFinal(Control_M)!=-1) &&
        !((rtmGetTFinal(Control_M)-Control_M->Timing.taskTime0) >
          Control_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(Control_M, "Simulation finished");
    }
  }

  // Update absolute time for base rate
  // The "clockTick0" counts the number of times the code of this task has
  //  been executed. The absolute time is the multiplication of "clockTick0"
  //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
  //  overflow during the application lifespan selected.

  Control_M->Timing.taskTime0 =
    ((time_T)(++Control_M->Timing.clockTick0)) * Control_M->Timing.stepSize0;
}

// Model initialize function
void Control_initialize(void)
{
  // Registration code
  rtmSetTFinal(Control_M, 15.0);
  Control_M->Timing.stepSize0 = 0.3;

  // Setup for data logging
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (nullptr);
    Control_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  // Setup for data logging
  {
    rtliSetLogXSignalInfo(Control_M->rtwLogInfo, (nullptr));
    rtliSetLogXSignalPtrs(Control_M->rtwLogInfo, (nullptr));
    rtliSetLogT(Control_M->rtwLogInfo, "tout");
    rtliSetLogX(Control_M->rtwLogInfo, "");
    rtliSetLogXFinal(Control_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(Control_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(Control_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(Control_M->rtwLogInfo, 0);
    rtliSetLogDecimation(Control_M->rtwLogInfo, 1);
    rtliSetLogY(Control_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(Control_M->rtwLogInfo, (nullptr));
    rtliSetLogYSignalPtrs(Control_M->rtwLogInfo, (nullptr));
  }

  // Matfile logging
  rt_StartDataLoggingWithStartTime(Control_M->rtwLogInfo, 0.0, rtmGetTFinal
    (Control_M), Control_M->Timing.stepSize0, (&rtmGetErrorStatus(Control_M)));
}

// Model terminate function
void Control_terminate(void)
{
  // (no terminate code required)
}

//
// File trailer for generated code.
//
// [EOF]
//
