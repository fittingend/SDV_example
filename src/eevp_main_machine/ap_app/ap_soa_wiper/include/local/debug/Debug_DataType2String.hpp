#ifndef __DEBUG_DATATYPE_TO_STRING_H__
#define __DEBUG_DATATYPE_TO_STRING_H__

#include <string>
#include <Common.h>
#include <SoaDataType.h>

extern std::string getStringEnum_bool(const bool &val);
extern std::string getStringEnum_DeviceNormal_e(const DeviceNormal_e &val);
extern std::string getStringEnum_ErrorState_e(const ErrorState_e &val);
extern std::string getStringEnum_ClientLevel_e(const ClientLevel_e &val);
extern std::string getStringEnum_PowerState_e(const PowerState_e &val);
extern std::string getStringEnum_GearState_e(const GearState_e &val);
extern std::string getStringEnum_IgnState_e(const IgnState_e &val);
extern std::string getStringEnum_AccState_e(const AccState_e &val);

// DMS
extern std::string getStringEnum_DmsDriverInside_e(const DmsDriverInside_e &val);
extern std::string getStringEnum_DmsFatigue_e(const DmsFatigue_e &val);
extern std::string getStringEnum_DmsGazingDir_e(const DmsGazingDir_e &val);
extern std::string getStringEnum_DmsOnPhone_e(const DmsOnPhone_e &val);
extern std::string getStringEnum_DmsSmoking_e(const DmsSmoking_e &val);

// DriverSeat
extern std::string getStringEnum_DriverSeatDir_e(const DriverSeatDir_e &val);

// HVAC
extern std::string getStringEnum_HvacAirSource_e(const HvacAirSource_e &val);
extern std::string getStringEnum_HvacBlowDir_e(const HvacBlowDir_e &val);
extern std::string getString_HvacStatus_t(const HvacStatus_t &val);
extern std::string getString_HvacTemps_t(const HvacTemps_t &val);

// MoodLamp
extern std::string getStringEnum_MlmMoodMode_e(const MlmMoodMode_e &val);

// RearCurtain
extern std::string getStringEnum_RCtnMotorDir_e(const RCtnMotorDir_e &val);
extern std::string getStringEnum_RCtnState_e(const RCtnState_e &val);
extern std::string getStringEnum_RCtnPos_e(const RCtnPos_e &val);

// ROA
extern std::string getStringEnum_RoaRunningState_e(const RoaRunningState_e &val);
extern std::string getStringEnum_RoaMode_e(const RoaMode_e &val);
extern std::string getStringEnum_RoaSensorError_e(const RoaSensorError_e &val);

// SmartFilm
extern std::string getStringEnum_SmtfilmPos_e(const SmtfilmPos_e &val);
extern std::string getString_SmtfilmOpacities_t(const SmtfilmOpacities_t &val);

// Wiper
extern std::string getStringEnum_WiperOpMode_e(const WiperOpMode_e &val);

// Multi Function Switch
extern std::string getStringEnum_MfswWiperSw_e(const MfswWiperSw_e &val);
extern std::string getStringEnum_MfswWasherSw_e(const MfswWasherSw_e &val);
extern std::string getStringEnum_MfswWiperIntStepSw_e(const MfswWiperIntStepSw_e &val);
extern std::string getStringEnum_MfswWiperMistSw_e(const MfswWiperMistSw_e &val);

#endif
