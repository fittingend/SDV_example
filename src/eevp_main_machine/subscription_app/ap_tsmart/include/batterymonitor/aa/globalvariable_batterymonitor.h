#ifndef GLOBALVARIABLE_BATTERYMONITOR_H
#define GLOBALVARIABLE_BATTERYMONITOR_H

#include <cstdint>

/// @brief Data for event, BattEvent

//Rx
extern uint16_t	vInterfaceIDforDbg;
extern uint32_t	vDataSerialNumber;
extern uint32_t	vVehicleUniqueSnr;
extern uint32_t	vEMS_BatteryAvgTemperature;
extern uint32_t	vEMS_ChargerOutputVolt;
extern uint32_t	vEMS_ChargerOutputCurr;
extern uint32_t	vEMS_ReturnCode;
extern uint32_t	vEMS_ConnectionStatus;
extern uint32_t	vEMS_TargetVolt;
extern uint32_t	vEMS_TargetCurr;
extern uint32_t	vEMS_RemainHour;
extern uint32_t	vEMS_RemainMin;
extern uint32_t	vEMS_HVBattSOC;
extern uint32_t	vEMS_HVBattSOE;
extern uint32_t	vEMS_HVBattSOH;
extern uint32_t	vEMS_BattVolt;
extern uint32_t	vEMS_BattCurr;
extern uint32_t	vEMS_Obc_dcVolt;
extern uint32_t	vEMS_Obc_dcCurr;
extern uint32_t	vEMS_Avn_ChargeCurrSetSts;
extern uint32_t	vEMS_HVBatt_MainRlyCloseReq;
extern uint32_t	vEMS_HVBatt_preChrgRlyStatus;
extern uint32_t	vEMS_HVBatt_posRlyStatus;
extern uint32_t	vEMS_HVBatt_negRlyStatus;
extern uint32_t	vEMS_Avn_DcChargingTargetSetSts;
extern uint32_t	vEMS_Avn_AcChargingTargetSetSts;

//Tx
extern uint32_t vGetCount;
extern uint32_t vMissedCount;

extern uint32_t vLastSerialNumber;
extern uint32_t	vEMS_HVBatt_MainRlyCloseReq_Prev;
extern uint8_t vStartFlag;
extern uint8_t vFinishFlag;

#endif
