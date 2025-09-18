//
//  batterymonitor.h
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


#ifndef batterymonitor_subfunc_h_
#define batterymonitor_subfunc_h_
#include <stdbool.h>
#include <stdint.h>
// #include "complex_types.h"
#include <stddef.h>
#include <array>
#include <memory>
#include <cmath>
#include <ctime> //시간계산

//socket - start
#include <iostream>         
#include <sys/socket.h>     
#include <arpa/inet.h>      
#include <unistd.h>         
#include <cstring>          
#include <chrono>
#include <thread>
#define TCP_NODELAY 1
//socket - end 

#include "ara/log/logger.h"
#include "ara/log/log_stream.h"

// #include "eevp/bmsinfo/impl_type_struct_bmsinfo.h" //cannot use (matlab)

#include "subfunc/batterymonitor_types.h"
#include "subfunc/zero_crossing_types.h"
#include "subfunc/multiword_types.h"
#include "subfunc/uMultiWordCmp.h"
#include "subfunc/uMultiWordMul.h"
#include "subfunc/uMultiWordGe.h"
#include "subfunc/uMultiWordLe.h"
#include "subfunc/uMultiWord2Double.h"
#include "subfunc/mod_USSjf2FO.h"
#include "subfunc/mul_u32_hiSR.h"
#include "subfunc/mul_u32_sat.h"  
#include "subfunc/data/table/impl_type_appversion.h"
#include "subfunc/data/table/impl_type_complete_time.h"
#include "subfunc/data/table/impl_type_date.h"
#include "subfunc/data/table/impl_type_elapsed_time.h"
#include "subfunc/data/table/impl_type_finished_time.h"
#include "subfunc/data/table/impl_type_maxdata.h"
#include "subfunc/data/table/impl_type_mindata.h"
#include "subfunc/data/table/impl_type_rcdata.h"
#include "subfunc/data/table/impl_type_y_data.h"
#include "subfunc/msg/info/impl_type_t_msginfo.h"
#include "subfunc/bms/input/impl_type_inputdata.h"

#include "subfunc/bms/input/impl_type_inputdata.h"
#include "eevp/control/soamlm_proxy.h"
#include "MoodLampProxyImpl.h"

//시간계산 변수 시작
extern time_t now;
extern time_t finishtime;
extern time_t starttime;
extern time_t elapsedtime;
//시간계산 변수 끝

//socket structure 시작
struct Socket_Header
{
    uint16_t SEQ;
    uint16_t Retry_Cnt;
    uint16_t Length;
    uint16_t Res;
};

struct Socket_Data
{
    std::uint32_t VehicleUniqueSnr;
    std::uint32_t InterfaceIDforDbg; //moved
    data::table::AppVersion AppVer;
    data::table::Date Date;          //moved
    std::uint8_t ChargeStatus;
    std::uint8_t EMS_ConnectionStatus;
    std::uint16_t BattSOC;
    std::uint16_t DTE;
    std::uint16_t BatteryStatus;
    std::uint16_t SOH;
    std::uint16_t SOE;
    std::uint8_t TargetSOC;
    data::table::Complete_Time CompleteTime;
    data::table::Finished_Time FinishedTime;
    std::uint32_t GetCount;
    std::uint32_t MissedCount;
    std::uint16_t ChargeResult;
    std::uint16_t ChargedSOE;
    std::uint16_t ChargedSOC;
    msg::info::T_MSGINFO Current;
    msg::info::T_MSGINFO Voltage;
    msg::info::T_MSGINFO SOC;
    msg::info::T_MSGINFO Temp;
    data::table::Elapsed_Time ElapsedTime;
    std::uint32_t ChargeStopReason;
    // PARA_STRUCTURE(OutputData, InterfaceIDforDbg, VehicleUniqueSnr, AppVer, ChargeStatus, EMS_ConnectionStatus, Date, BattSOC, DTE, BatteryStatus, SOH, SOE, TargetSOC, CompleteTime, FinishedTime, GetCount, MissedCount, ChargeResult, ChargedSOE, ChargedSOC, Current, Voltage, SOC, Temp, ElapsedTime, ChargeStopReason);
};

struct Socket_Frame
{
    uint16_t       STX;
    Socket_Header socket_header;
    Socket_Data   socket_data;
    uint16_t       ETX;
};

//socket structure 끝

  struct B_batterymonitor_T {
    bms::input::InputData RxTriggered; // '<S1>/RxTriggered'
    uint8_t OutportBufferForCurrenttRslt;// '<S17>/Data Type Conversion26'
    uint8_t OutportBufferForVolttageRslt;// '<S17>/Data Type Conversion27'
    uint8_t OutportBufferForSOCtRslt;  // '<S17>/Data Type Conversion24'
    uint8_t OutportBufferForTempRslt;  // '<S17>/Data Type Conversion25'
    std::array<uint8_t, 100> Ydata;    // '<S17>/Convert_VoltYdata'
    std::array<uint8_t, 20> MAXdata;   // '<S17>/Convert_VoltRCdata'
    std::array<uint8_t, 2> MINdata;    // '<S17>/Convert_VoltMINdata'
    std::array<uint8_t, 2> MAXdata_m;  // '<S17>/Convert_VoltMAXdata'
    std::array<uint8_t, 100> Ydata_j;  // '<S17>/Convert_TempYdata'
    std::array<uint8_t, 20> MAXdata_k; // '<S17>/Convert_TempRCdata'
    std::array<uint8_t, 2> MINdata_g;  // '<S17>/Convert_TempMINdata'
    std::array<uint8_t, 2> MAXdata_i;  // '<S17>/Convert_TempMAXdata'
    std::array<uint8_t, 100> Ydata_h;  // '<S17>/Convert_SOCYdata1'
    std::array<uint8_t, 20> MAXdata_j; // '<S17>/Convert_SOCRCdata'
    std::array<uint8_t, 2> MINdata_i;  // '<S17>/Convert_SOCMINdata1'
    std::array<uint8_t, 2> MAXdata_p;  // '<S17>/Convert_SOCMAXdata1'
    std::array<uint8_t, 100> Ydata_p;  // '<S17>/Convert_CurrYdata'
    std::array<uint8_t, 20> MAXdata_b; // '<S17>/Convert_CurrRCdata'
    std::array<uint8_t, 2> MINdata_iq; // '<S17>/Convert_CurrMINdata'
    std::array<uint8_t, 2> MAXdata_o;  // '<S17>/Convert_CurrMAXdata'
    bool FixPtRelationalOperator;      // '<S51>/FixPt Relational Operator'
    bool FixPtRelationalOperator_p;    // '<S50>/FixPt Relational Operator'
  };

  // Block states (default storage) for system '<Root>'
  struct DW_batterymonitor_T {
    double DelayInput1_DSTATE;         // '<S51>/Delay Input1'
    double UnitDelay3_DSTATE;          // '<S2>/Unit Delay3'
    std::array<double, 100> UnitDelay12_DSTATE;// '<S17>/Unit Delay12'
    std::array<double, 20> UnitDelay37_DSTATE;// '<S17>/Unit Delay37'
    std::array<double, 2> UnitDelay9_DSTATE;// '<S17>/Unit Delay9'
    std::array<double, 2> UnitDelay10_DSTATE;// '<S17>/Unit Delay10'
    double UnitDelay13_DSTATE;         // '<S17>/Unit Delay13'
    std::array<double, 100> UnitDelay19_DSTATE;// '<S17>/Unit Delay19'
    std::array<double, 20> UnitDelay39_DSTATE;// '<S17>/Unit Delay39'
    std::array<double, 2> UnitDelay16_DSTATE;// '<S17>/Unit Delay16'
    std::array<double, 2> UnitDelay17_DSTATE;// '<S17>/Unit Delay17'
    double UnitDelay20_DSTATE;         // '<S17>/Unit Delay20'
    std::array<double, 100> UnitDelay26_DSTATE;// '<S17>/Unit Delay26'
    std::array<double, 20> UnitDelay41_DSTATE;// '<S17>/Unit Delay41'
    std::array<double, 2> UnitDelay23_DSTATE;// '<S17>/Unit Delay23'
    std::array<double, 2> UnitDelay24_DSTATE;// '<S17>/Unit Delay24'
    double UnitDelay27_DSTATE;         // '<S17>/Unit Delay27'
    std::array<double, 100> UnitDelay5_DSTATE;// '<S17>/Unit Delay5'
    std::array<double, 20> UnitDelay43_DSTATE;// '<S17>/Unit Delay43'
    std::array<double, 2> UnitDelay2_DSTATE;// '<S17>/Unit Delay2'
    std::array<double, 2> UnitDelay3_DSTATE_h;// '<S17>/Unit Delay3'
    double UnitDelay6_DSTATE;          // '<S17>/Unit Delay6'
    uint32_t DelayInput1_DSTATE_n;     // '<S50>/Delay Input1'
    uint32_t UnitDelay1_DSTATE;        // '<S2>/Unit Delay1'
    uint32_t UnitDelay_DSTATE;         // '<S2>/Unit Delay'
    uint32_t UnitDelay2_DSTATE_a;      // '<S2>/Unit Delay2'
    uint32_t UnitDelay5_DSTATE_k;      // '<S2>/Unit Delay5'
    uint32_t UnitDelay4_DSTATE;        // '<S2>/Unit Delay4'
    uint32_t UnitDelay28_DSTATE;       // '<S17>/Unit Delay28'
    uint32_t UnitDelay30_DSTATE;       // '<S17>/Unit Delay30'
    uint32_t UnitDelay32_DSTATE;       // '<S17>/Unit Delay32'
    uint32_t UnitDelay34_DSTATE;       // '<S17>/Unit Delay34'
    uint32_t UnitDelay35_DSTATE;       // '<S17>/Unit Delay35'
    uint16_t UnitDelay1_DSTATE_m;      // '<S4>/Unit Delay1'
    uint16_t UnitDelay2_DSTATE_k;      // '<S4>/Unit Delay2'
    uint8_t UnitDelay7_DSTATE;         // '<S17>/Unit Delay7'
    uint8_t UnitDelay29_DSTATE;        // '<S17>/Unit Delay29'
    uint8_t UnitDelay11_DSTATE;        // '<S17>/Unit Delay11'
    uint8_t UnitDelay8_DSTATE;         // '<S17>/Unit Delay8'
    uint8_t UnitDelay36_DSTATE;        // '<S17>/Unit Delay36'
    uint8_t UnitDelay46_DSTATE;        // '<S17>/Unit Delay46'
    uint8_t UnitDelay14_DSTATE;        // '<S17>/Unit Delay14'
    uint8_t UnitDelay31_DSTATE;        // '<S17>/Unit Delay31'
    uint8_t UnitDelay18_DSTATE;        // '<S17>/Unit Delay18'
    uint8_t UnitDelay15_DSTATE;        // '<S17>/Unit Delay15'
    uint8_t UnitDelay38_DSTATE;        // '<S17>/Unit Delay38'
    uint8_t UnitDelay47_DSTATE;        // '<S17>/Unit Delay47'
    uint8_t UnitDelay21_DSTATE;        // '<S17>/Unit Delay21'
    uint8_t UnitDelay33_DSTATE;        // '<S17>/Unit Delay33'
    uint8_t UnitDelay25_DSTATE;        // '<S17>/Unit Delay25'
    uint8_t UnitDelay22_DSTATE;        // '<S17>/Unit Delay22'
    uint8_t UnitDelay40_DSTATE;        // '<S17>/Unit Delay40'
    uint8_t UnitDelay44_DSTATE;        // '<S17>/Unit Delay44'
    uint8_t UnitDelay_DSTATE_g;        // '<S17>/Unit Delay'
    uint8_t UnitDelay4_DSTATE_m;       // '<S17>/Unit Delay4'
    uint8_t UnitDelay1_DSTATE_k;       // '<S17>/Unit Delay1'
    uint8_t UnitDelay42_DSTATE;        // '<S17>/Unit Delay42'
    uint8_t UnitDelay45_DSTATE;        // '<S17>/Unit Delay45'
  };

  // Zero-crossing (trigger) state
  struct PrevZCX_batterymonitor_T {
    ZCSigState Subsystem1_Trig_ZCE;    // '<S1>/Subsystem1'
    ZCSigState Subsystem_Trig_ZCE;     // '<S1>/Subsystem'
    ZCSigState SendMessage_Trig_ZCE;   // '<S1>/SendMessage'
  };

  // Triggered events
  extern PrevZCX_batterymonitor_T batterymonitor_PrevZCX;

  // Block signals
  extern B_batterymonitor_T batterymonitor_B;

  // Block states
  extern DW_batterymonitor_T batterymonitor_DW;

  //시간계산 서브함수 시작
  int toBCD(int num) ;
  //시간계산 서브함수 끝

  // model step function
  void BatteryMonitor_triggered_sys(B_batterymonitor_T batterymonitor_B, uint8_t *brightness, Socket_Data* socket_data);

  //Subfunction for Socket - Not made by Simulink
  void SendSocket(Socket_Data socket_data);

  // private member function(s) for subsystem '<S17>/Convert_CurrMAXdata'
  static void batterymonitor_Convert_CurrMAXdata(const double rtu_u[2], uint8_t rty_MAXdata[2]);

  // private member function(s) for subsystem '<S17>/Convert_CurrMINdata'
  static void batterymonitor_Convert_CurrMINdata();

  // private member function(s) for subsystem '<S17>/Convert_CurrRCdata'
  static void batterymonitor_Convert_CurrRCdata(const double rtu_u[20], uint8_t rty_MAXdata[20]);

  // private member function(s) for subsystem '<S17>/Convert_CurrYdata'
  static void batterymonitor_Convert_CurrYdata(const double rtu_u[100], uint8_t rty_Ydata[100]);

  // private member function(s) for subsystem '<S17>/MATLAB Function'
  static void batterymonitor_MATLABFunction(uint8_t rtu_y_prev, uint8_t rtu_val,
    double rtu_val_min, double rtu_val_max, uint8_t rtu_der, double rtu_der_min,
    double rtu_der_max, double rtu_ChargeStatus, double rtu_ChargeStatus_prev,
    double *rty_y);

  // private member function(s) for subsystem '<S17>/MATLAB Function10'
  static void batterymonitor_MATLABFunction10(uint8_t rtu_der, double
    rtu_der_min, double rtu_der_max, double *rty_RCActive);

  // private member function(s) for subsystem '<S17>/MATLAB Function4'
  static void batterymonitor_MATLABFunction4(uint32_t rtu_t, uint32_t rtu_t_prev,
    uint8_t rtu_val, uint8_t rtu_val_prev, double *rty_y);

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S2>/Data Type Conversion1' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion10' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion12' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion14' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion5' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion6' : Eliminatbatterymonitor_sue redundant data type conversion
//  Block '<S2>/Data Type Conversion7' : Eliminate redundant data type conversion
//  Block '<S2>/Data Type Conversion9' : Eliminate redundant data type conversion


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'batterymonitor'
//  '<S1>'   : 'batterymonitor/BatteryMonitor_triggered_sys'
//  '<S2>'   : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData'
//  '<S3>'   : 'batterymonitor/BatteryMonitor_triggered_sys/Determine_SendMessage'
//  '<S4>'   : 'batterymonitor/BatteryMonitor_triggered_sys/SendMessage'
//  '<S5>'   : 'batterymonitor/BatteryMonitor_triggered_sys/Subsystem'
//  '<S6>'   : 'batterymonitor/BatteryMonitor_triggered_sys/Subsystem1'
//  '<S7>'   : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Calculate_CompleteTime'
//  '<S8>'   : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Calculate_DTE'
//  '<S9>'   : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Calculate_FinishedTime'
//  '<S10>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Calculate_GetCount'
//  '<S11>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Calculate_MissCount'
//  '<S12>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Calculate_PresentTime'
//  '<S13>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Determine_AppVersion'
//  '<S14>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Determine_ChargeResult'
//  '<S15>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Determine_ChargeStatus'
//  '<S16>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/MATLAB Function2'
//  '<S17>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1'
//  '<S18>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Calculate_CurrInfo'
//  '<S19>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Calculate_SOCInfo'
//  '<S20>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Calculate_TempInfo'
//  '<S21>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Calculate_VoltInfo'
//  '<S22>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_CurrMAXdata'
//  '<S23>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_CurrMINdata'
//  '<S24>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_CurrRCdata'
//  '<S25>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_CurrYdata'
//  '<S26>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_SOCMAXdata1'
//  '<S27>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_SOCMINdata1'
//  '<S28>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_SOCRCdata'
//  '<S29>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_SOCYdata1'
//  '<S30>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_TempMAXdata'
//  '<S31>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_TempMINdata'
//  '<S32>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_TempRCdata'
//  '<S33>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_TempYdata'
//  '<S34>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_VoltMAXdata'
//  '<S35>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_VoltMINdata'
//  '<S36>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_VoltRCdata'
//  '<S37>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/Convert_VoltYdata'
//  '<S38>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function'
//  '<S39>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function1'
//  '<S40>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function10'
//  '<S41>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function11'
//  '<S42>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function2'
//  '<S43>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function3'
//  '<S44>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function4'
//  '<S45>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function5'
//  '<S46>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function6'
//  '<S47>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function7'
//  '<S48>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function8'
//  '<S49>'  : 'batterymonitor/BatteryMonitor_triggered_sys/CollectChargeData/Subsystem1/MATLAB Function9'
//  '<S50>'  : 'batterymonitor/BatteryMonitor_triggered_sys/Determine_SendMessage/Detect Decrease'
//  '<S51>'  : 'batterymonitor/BatteryMonitor_triggered_sys/Determine_SendMessage/Detect Increase'
//  '<S52>'  : 'batterymonitor/BatteryMonitor_triggered_sys/Determine_SendMessage/MATLAB Function'
//  '<S53>'  : 'batterymonitor/BatteryMonitor_triggered_sys/SendMessage/MATLAB Function12'


#endif                                 // batterymonitor_subfunc_h_
