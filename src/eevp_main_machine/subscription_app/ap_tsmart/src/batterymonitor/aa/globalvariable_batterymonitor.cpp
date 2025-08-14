#include "batterymonitor/aa/globalvariable_batterymonitor.h"

//Rx
uint16_t	vInterfaceIDforDbg = 0;
uint32_t	vDataSerialNumber = 0;
uint32_t	vVehicleUniqueSnr = 0;
uint32_t	vEMS_BatteryAvgTemperature = 0;
uint32_t	vEMS_ChargerOutputVolt = 0;
uint32_t	vEMS_ChargerOutputCurr = 0;
uint32_t	vEMS_ReturnCode = 0;
uint32_t	vEMS_ConnectionStatus = 0;
uint32_t	vEMS_TargetVolt = 0;
uint32_t	vEMS_TargetCurr = 0;
uint32_t	vEMS_RemainHour = 0;
uint32_t	vEMS_RemainMin = 0;
uint32_t	vEMS_HVBattSOC = 0;
uint32_t	vEMS_HVBattSOE = 0;
uint32_t	vEMS_HVBattSOH = 0;
uint32_t	vEMS_BattVolt = 0;
uint32_t	vEMS_BattCurr = 0;
uint32_t	vEMS_Obc_dcVolt = 0;
uint32_t	vEMS_Obc_dcCurr = 0;
uint32_t	vEMS_Avn_ChargeCurrSetSts = 0;
uint32_t	vEMS_HVBatt_MainRlyCloseReq = 0;
uint32_t	vEMS_HVBatt_preChrgRlyStatus = 0;
uint32_t	vEMS_HVBatt_posRlyStatus = 0;
uint32_t	vEMS_HVBatt_negRlyStatus = 0;
uint32_t	vEMS_Avn_DcChargingTargetSetSts = 0;
uint32_t	vEMS_Avn_AcChargingTargetSetSts = 0;

//Tx
uint32_t    vGetCount = 0;
uint32_t    vMissedCount = 0;

uint32_t    vLastSerialNumber = 0;
uint32_t	vEMS_HVBatt_MainRlyCloseReq_Prev = 0;
uint8_t     vStartFlag = 0;
uint8_t     vFinishFlag = 0;