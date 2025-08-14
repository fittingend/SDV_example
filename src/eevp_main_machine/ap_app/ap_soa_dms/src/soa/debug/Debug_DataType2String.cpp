#include <Common.h>
#include <SoaDataType.h>
#include <string>
#include <sstream>
#include <Debug_DataType2String.hpp>

std::string getStringEnum_bool(const bool &val)
{
	return (val) ? "true" : "false";
}

std::string getStringEnum_DeviceNormal_e(const DeviceNormal_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DEVICE_NORMAL
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_ErrorState_e(const ErrorState_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_ERROR_STATE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_ClientLevel_e(const ClientLevel_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_CLIENT_LEVEL
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_PowerState_e(const PowerState_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_POWER_STATE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_GearState_e(const GearState_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_GEAR_STATE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_IgnState_e(const IgnState_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_IGN_STATE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_AccState_e(const AccState_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_ACC_STATE
	#undef X
	default:
		break;
	}

	return "N/A";
}

/*
	DMS
*/

std::string getStringEnum_DmsDriverInside_e(const DmsDriverInside_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DMS_DRIVER_INSIDE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_DmsFatigue_e(const DmsFatigue_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DMS_FATIGUE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_DmsGazingDir_e(const DmsGazingDir_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DMS_GAZING_DIR
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_DmsOnPhone_e(const DmsOnPhone_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DMS_ON_PHONE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_DmsSmoking_e(const DmsSmoking_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DMS_SMOKING
	#undef X
	default:
		break;
	}

	return "N/A";
}

/*
	DriverSeat
*/
std::string getStringEnum_DriverSeatDir_e(const DriverSeatDir_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_DRIVERSEAT_DIR
	#undef X
	default:
		break;
	}

	return "N/A";
}

/*
	HVAC
*/
std::string getStringEnum_HvacAirSource_e(const HvacAirSource_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_HVAC_AIRSOURCE
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_HvacBlowDir_e(const HvacBlowDir_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_HVAC_BLOWDIR
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getString_HvacStatus_t(const HvacStatus_t &val)
{
	std::ostringstream ss;
	ss << "mPowerOn=" << getStringEnum_bool(val.mPowerOn);
	ss << ", mAcOnOff=" << getStringEnum_bool(val.mAcOnOff);
	ss << ", mAutoMode=" << getStringEnum_bool(val.mAutoMode);
	ss << ", mSyncMode=" << getStringEnum_bool(val.mSyncMode);
	ss << ", mDriverTemp=" << val.mDriverTemp;
	ss << ", mPassengerTemp=" << val.mPassengerTemp;
	ss << ", mBlowingForce=" << val.mBlowingForce;
	ss << ", mBlowDir=" << getStringEnum_HvacBlowDir_e(val.mBlowDir);
	ss << ", mAirSrc=" << getStringEnum_HvacAirSource_e(val.mAirSrc);
	ss << ", mForwardFrostOn=" << getStringEnum_bool(val.mForwardFrostOn);
	ss << ", mBackwardFrostOn=" << getStringEnum_bool(val.mBackwardFrostOn);
	return ss.str();
}

std::string getString_HvacTemps_t(const HvacTemps_t &val)
{
	std::ostringstream ss;
	ss << "mInside=" << val.mInside;
	ss << ", mOutside=" << val.mOutside;
	ss << ", mDuct=" << val.mDuct;
	ss << ", mEvaporator=" << val.mEvaporator;
	ss << ", mDriverVentDuct=" << val.mDriverVentDuct;
	ss << ", mDriverFloorDuct=" << val.mDriverFloorDuct;
	ss << ", mPassengerVentDuct=" << val.mPassengerVentDuct;
	ss << ", mPassengerFloorDuct=" << val.mPassengerFloorDuct;
	return ss.str();
}

/*
	MoodLamp
*/

std::string getStringEnum_MlmMoodMode_e(const MlmMoodMode_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_MOODLAMP_MODE
	#undef X
	default:
		break;
	}

	return "N/A";
}

/*
	RearCurtain
*/

std::string getStringEnum_RCtnMotorDir_e(const RCtnMotorDir_e &val)
{
	switch (val)
	{
#define X(name, value) case name: return #name;
	ENUM_LIST_RCTN_MOTOR_DIR
#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_RCtnState_e(const RCtnState_e &val)
{
	switch (val)
	{
#define X(name, value) case name: return #name;
	ENUM_LIST_RCTN_STATE
#undef X
	default:
		break;
	}

	return "N/A";

}

std::string getStringEnum_RCtnPos_e(const RCtnPos_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_RCTN_POS
	#undef X
	default:
		break;
	}

	return "N/A";
}

/*
	ROA
*/

std::string getStringEnum_RoaRunningState_e(const RoaRunningState_e &val)
{
	switch (val)
	{
#define X(name, value) case name: return #name;
	ENUM_LIST_ROA_RUNNINGSTATE
#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_RoaMode_e(const RoaMode_e &val)
{
	switch (val)
	{
#define X(name, value) case name: return #name;
	ENUM_LIST_ROA_MODE
#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_RoaSensorError_e(const RoaSensorError_e &val)
{
	switch (val)
	{
#define X(name, value) case name: return #name;
	ENUM_LIST_ROA_SENSORERROR
#undef X
	default:
		break;
	}

	return "N/A";
}


/*
	SmartFilm
*/

std::string getStringEnum_SmtfilmPos_e(const SmtfilmPos_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_SMTFILM_POS
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getString_SmtfilmOpacities_t(const SmtfilmOpacities_t &val)
{
	std::ostringstream ss;
	ss << "WindShield=" << val.mOpacities[eSmtfilmPos_WindShield];
	ss << ", RoofGlass=" << val.mOpacities[eSmtfilmPos_RoofGlass];
	ss << ", DsFrontRear=" << val.mOpacities[eSmtfilmPos_DsFrontRear];
	ss << ", DsQuarter=" << val.mOpacities[eSmtfilmPos_DsQuarter];
	ss << ", AsFrontRear=" << val.mOpacities[eSmtfilmPos_AsFrontRear];
	ss << ", AsQuarter=" << val.mOpacities[eSmtfilmPos_AsQuarter];
	ss << ", RearGrass=" << val.mOpacities[eSmtfilmPos_RearGrass];
	return ss.str();
}


/*
	Wiper
*/

std::string getStringEnum_WiperOpMode_e(const WiperOpMode_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_WIPER_OPMODE
	#undef X
	default:
		break;
	}

	return "N/A";
}


/*
	Multifunction Switch
*/

std::string getStringEnum_MfswWiperSw_e(const MfswWiperSw_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_MFSW_WIPER_SWITCH
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_MfswWasherSw_e(const MfswWasherSw_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_MFSW_WASHER_SWITCH
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_MfswWiperIntStepSw_e(const MfswWiperIntStepSw_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_MFSW_WIPER_INTSTEP_SWITCH
	#undef X
	default:
		break;
	}

	return "N/A";
}

std::string getStringEnum_MfswWiperMistSw_e(const MfswWiperMistSw_e &val)
{
	switch (val)
	{
	#define X(name, value) case name: return #name;
	ENUM_LIST_MFSW_WIPER_MIST_SWITCH
	#undef X
	default:
		break;
	}

	return "N/A";
}
