#ifndef __SOA_DATA_TYPE_H__
#define __SOA_DATA_TYPE_H__

#include <Common.h>
#include <cstdint>
#include <string.h>

/*
	Common DataTypes
*/

// DeviceNormal_e
#define ENUM_LIST_DEVICE_NORMAL \
	X(eDeviceNormal_Ok, 0) \
	X(eDeviceNormal_Abnormal, 1) \
	X(eDeviceNormal_NotAvailable, 127)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_DEVICE_NORMAL
	#undef X
} DeviceNormal_e;

#define ENUM_LIST_ERROR_STATE \
	X(eERROR_STATE_Ok, 0) \
	X(eERROR_STATE_Error, 1)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_ERROR_STATE
	#undef X
} ErrorState_e;

// ClientLevel_e
#define ENUM_LIST_CLIENT_LEVEL \
	X(eClientLevel_SubscribedApps, 0) \
	X(eClientLevel_GuiApps, 1) \
	X(eClientLevel_PhysicalSwitch, 2)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_CLIENT_LEVEL
	#undef X
} ClientLevel_e;

// PowerState_e
#define ENUM_LIST_POWER_STATE \
	X(ePowerState_Off, 0) \
	X(ePowerState_On, 1) \
	X(ePowerState_Error, 2)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_POWER_STATE
	#undef X
} PowerState_e;

// GearState_e
#define ENUM_LIST_GEAR_STATE \
	X(eGearState_P, 1) \
	X(eGearState_D, 2) \
	X(eGearState_N, 3) \
	X(eGearState_R, 4) \
	X(eGearState_L, 5) \
	X(eGearState_2, 6) \
	X(eGearState_3, 7) \
	X(eGearState_DS, 8) \
	X(eGearState_ManualShift, 9) \
	X(eGearState_PaddleShift, 10) \
	X(eGearState_SubcomComm, 11) \
	X(eGearState_Fault, 12) \
	X(eGearState_Unknown, 127)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_GEAR_STATE
	#undef X
} GearState_e;

// IgnState_e
#define ENUM_LIST_IGN_STATE \
	X(eIgnState_Off, 0) \
	X(eIgnState_On, 1) \
	X(eIgnState_Error, 2) \
	X(eIgnState_Unknown, 127)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_IGN_STATE
	#undef X
} IgnState_e;

// AccState_e
#define ENUM_LIST_ACC_STATE \
	X(eAccState_Off, 0) \
	X(eAccState_On, 1) \
	X(eAccState_Error, 2) \
	X(eAccState_Unknown, 127)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_ACC_STATE
	#undef X
} AccState_e;

/*
	DMS DataTypes
*/

// DmsDriverInside_e
#define ENUM_LIST_DMS_DRIVER_INSIDE \
	X(eDmsDriverInside_Unknown, 0) \
	X(eDmsDriverInside_DriverIn, 1) \
	X(eDmsDriverInside_None, 2)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_DMS_DRIVER_INSIDE
	#undef X
} DmsDriverInside_e;

// DmsFatigue_e
#define ENUM_LIST_DMS_FATIGUE \
	X(eDmsFatigue_Unknown, 0) \
	X(eDmsFatigue_Ok, 1) \
	X(eDmsFatigue_Drowsy, 2) \
	X(eDmsFatigue_Sleep, 3)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_DMS_FATIGUE
	#undef X
} DmsFatigue_e;

// DmsGazingDir_e
#define ENUM_LIST_DMS_GAZING_DIR \
	X(eDmsGazingDir_Unknown, 0) \
	X(eDmsGazingDir_Front, 1) \
	X(eDmsGazingDir_FrontLeft, 2) \
	X(eDmsGazingDir_FrontRight, 3) \
	X(eDmsGazingDir_RearMirror, 4) \
	X(eDmsGazingDir_LeftMirror, 5) \
	X(eDmsGazingDir_RightMirror, 6) \
	X(eDmsGazingDir_DashBoard, 7) \
	X(eDmsGazingDir_Media, 8) \
	X(eDmsGazingDir_Other, 9)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_DMS_GAZING_DIR
	#undef X
} DmsGazingDir_e;

// DmsOnPhone_e
#define ENUM_LIST_DMS_ON_PHONE \
	X(eDmsOnPhone_Unknown, 0) \
	X(eDmsOnPhone_OnPhone, 1) \
	X(eDmsOnPhone_NotOnPhone, 2)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_DMS_ON_PHONE
	#undef X
} DmsOnPhone_e;

// DmsSmoking_e
#define ENUM_LIST_DMS_SMOKING \
	X(eDmsSmoking_Unknown, 0) \
	X(eDmsSmoking_Smoking, 1) \
	X(eDmsSmoking_NoSmoking, 2)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_DMS_SMOKING
	#undef X
} DmsSmoking_e;



/*
	DriverSeat DataTypes
*/


#define ENUM_LIST_DRIVERSEAT_DIR \
	X(eDriverSeatDir_Stop, 0) \
	X(eDriverSeatDir_Up, 1) \
	X(eDriverSeatDir_Down, 2) \
	X(eDriverSeatDir_Forward, 3) \
	X(eDriverSeatDir_Backward, 4) \
	X(eDriverSeatDir_NotAvailable, 5)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_DRIVERSEAT_DIR
#undef X
} DriverSeatDir_e;



/*
	HVAC DataTypes
*/

// HvacAirSource_e
#define ENUM_LIST_HVAC_AIRSOURCE \
	X(eHvacAirSource_Outer, 0) \
	X(eHvacAirSource_Inner, 1)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_HVAC_AIRSOURCE
	#undef X
} HvacAirSource_e;

// HvacBlowDir_e
#define ENUM_LIST_HVAC_BLOWDIR \
	X(eHvacBlowDir_Chest, 0) \
	X(eHvacBlowDir_Both, 1) \
	X(eHvacBlowDir_Foot, 2) \
	X(eHvacBlowDir_FootWithWS, 3)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_HVAC_BLOWDIR
	#undef X
} HvacBlowDir_e;

// HvacStatus_t
typedef struct _HvacStatus_t_ {
	bool mPowerOn;
	bool mAcOnOff;
	bool mAutoMode;
	bool mSyncMode;
	int mDriverTemp;
	int mPassengerTemp;
	int mBlowingForce;
	HvacBlowDir_e mBlowDir;
	HvacAirSource_e mAirSrc;
	bool mForwardFrostOn;
	bool mBackwardFrostOn;

	_HvacStatus_t_() {
		mPowerOn = false;
		mAcOnOff = false;
		mAutoMode = false;
		mSyncMode = false;
		mDriverTemp = 0;
		mPassengerTemp = 0;
		mBlowingForce = 0;
		mBlowDir = eHvacBlowDir_Chest;
		mAirSrc = eHvacAirSource_Inner;
		mForwardFrostOn = false;
		mBackwardFrostOn = false;
	}

	_HvacStatus_t_& operator=(const _HvacStatus_t_ &other) {
		if (this != &other)
		{
			mPowerOn = other.mPowerOn;
			mAcOnOff = other.mAcOnOff;
			mAutoMode = other.mAutoMode;
			mSyncMode = other.mSyncMode;
			mDriverTemp = other.mDriverTemp;
			mPassengerTemp = other.mPassengerTemp;
			mBlowingForce = other.mBlowingForce;
			mBlowDir = other.mBlowDir;
			mAirSrc = other.mAirSrc;
			mForwardFrostOn = other.mForwardFrostOn;
			mBackwardFrostOn = other.mBackwardFrostOn;
		}

		return *this;
	}

	bool operator==(const _HvacStatus_t_& other) const {
		return ((mPowerOn == other.mPowerOn) && (mAcOnOff == other.mAcOnOff) && (mAutoMode == other.mAutoMode) && (mSyncMode == other.mSyncMode) \
			&& (mDriverTemp == other.mDriverTemp) && (mPassengerTemp == other.mPassengerTemp) && (mBlowingForce == other.mBlowingForce) \
			&& (mBlowDir == other.mBlowDir) && (mAirSrc == other.mAirSrc) && (mForwardFrostOn == other.mForwardFrostOn) && (mBackwardFrostOn == other.mBackwardFrostOn));
	}

} HvacStatus_t;

// HvacTemps_t
typedef struct _HvacTemps_t_ {
	int mInside;
	int mOutside;
	int mDuct;
	int mEvaporator;
	int mDriverVentDuct;
	int mDriverFloorDuct;
	int mPassengerVentDuct;
	int mPassengerFloorDuct;

	_HvacTemps_t_() {
		mInside = 0;
		mOutside = 0;
		mDuct = 0;
		mEvaporator = 0;
		mDriverVentDuct = 0;
		mDriverFloorDuct = 0;
		mPassengerVentDuct = 0;
		mPassengerFloorDuct = 0;
	}

	_HvacTemps_t_& operator=(const _HvacTemps_t_ &other) {
		if (this != &other)
		{
			mInside = other.mInside;
			mOutside = other.mOutside;
			mDuct = other.mDuct;
			mEvaporator = other.mEvaporator;
			mDriverVentDuct = other.mDriverVentDuct;
			mDriverFloorDuct = other.mDriverFloorDuct;
			mPassengerVentDuct = other.mPassengerVentDuct;
			mPassengerFloorDuct = other.mPassengerFloorDuct;
		}

		return *this;
	}

	bool operator==(const _HvacTemps_t_& other) const {
		return ((mInside == other.mInside) && (mOutside == other.mOutside) && (mDuct == other.mDuct) && (mEvaporator == other.mEvaporator) \
			&& (mDriverVentDuct == other.mDriverVentDuct) && (mDriverFloorDuct == other.mDriverFloorDuct)\
			&& (mPassengerVentDuct == other.mPassengerVentDuct) && (mPassengerFloorDuct == other.mPassengerFloorDuct));
	}

} HvacTemps_t;



/*
	MoodLamp DataTypes
*/

// MlmMoodMode_e
#define ENUM_LIST_MOODLAMP_MODE \
	X(eMlmMoodMode_NoMood, 0) \
	X(eMlmMoodMode_CareMood, 1) \
	X(eMlmMoodMode_ComfortMood, 2) \
	X(eMlmMoodMode_DelightMood, 3) \
	X(eMlmMoodMode_MoodTherapy, 4) \
	X(eMlmMoodMode_VitalityMood, 5)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_MOODLAMP_MODE
	#undef X
} MlmMoodMode_e;



/*
	RearCurtain DataType
*/

// RCtnSwitch_e
#define ENUM_LIST_RCTN_MOTOR_DIR \
	X(eRCtnMotorDir_Off, 0) \
	X(eRCtnMotorDir_OpenOn, 1) \
	X(eRCtnMotorDir_CloseOn, 2) \
	X(eRCtnMotorDir_Unknown, 127)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_RCTN_MOTOR_DIR
	#undef X
} RCtnMotorDir_e;

// RCtnState_e
#define ENUM_LIST_RCTN_STATE \
	X(eRCtnState_FullyOpened, 0) \
	X(eRCtnState_FullyClosed, 1) \
	X(eRCtnState_PartlyOpened, 2) \
	X(eRCtnState_Opening, 3) \
	X(eRCtnState_Closing, 4) \
	X(eRCtnState_Unknown, 5) \
	X(eRCtnState_Error, 6)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_RCTN_STATE
#undef X
} RCtnState_e;

#define ENUM_LIST_RCTN_POS \
	X(eRCtnPos_Unknown, 0) \
	X(eRCtnPos_FullyClosed, 1) \
	X(eRCtnPos_Closed66, 2) \
	X(eRCtnPos_Closed33, 3) \
	X(eRCtnPos_FullyOpened, 4)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_RCTN_POS
	#undef X
} RCtnPos_e;



/*
	ROA DataTypes
*/

#define ENUM_LIST_ROA_RUNNINGSTATE \
	X(eRoaRunningState_Off, 0) \
	X(eRoaRunningState_On, 1) \
	X(eRoaRunningState_Error, 2)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_ROA_RUNNINGSTATE
#undef X
} RoaRunningState_e;

#define ENUM_LIST_ROA_MODE \
	X(eRoaMode_Off, 0) \
	X(eRoaMode_Normal, 1) \
	X(eRoaMode_Advanced, 2)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_ROA_MODE
#undef X
} RoaMode_e;

#define ENUM_LIST_ROA_SENSORERROR \
	X(eRoaSensorError_Ok, 0) \
	X(eRoaSensorError_Snsr, 1) \
	X(eRoaSensorError_Blckge, 2) \
	X(eRoaSensorError_Unknown, 3)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_ROA_SENSORERROR
#undef X
} RoaSensorError_e;



/*
	SmartFilm DataTypes
*/

// SmtfilmPos_e
#define ENUM_LIST_SMTFILM_POS \
	X(eSmtfilmPos_WindShield, 1) \
	X(eSmtfilmPos_RoofGlass, 2) \
	X(eSmtfilmPos_DsFrontRear, 3) \
	X(eSmtfilmPos_DsQuarter, 4) \
	X(eSmtfilmPos_AsFrontRear, 5) \
	X(eSmtfilmPos_AsQuarter, 6) \
	X(eSmtfilmPos_RearGrass, 7) \
	X(eSmtfilePos_MAX, 16)

typedef enum {
	#define X(name,value) name=value,
	ENUM_LIST_SMTFILM_POS
	#undef X
} SmtfilmPos_e;

typedef struct _SmtfilmOpacities_t_ {
	int mOpacities[eSmtfilePos_MAX];
	_SmtfilmOpacities_t_() {
		memset(mOpacities, 0, sizeof(int) * eSmtfilePos_MAX);
	}

	_SmtfilmOpacities_t_ & operator=(const _SmtfilmOpacities_t_ &other) {
		if (this != &other)
		{
			memcpy(mOpacities, other.mOpacities, sizeof(int) * eSmtfilePos_MAX);
		}

		return *this;
	}

	bool operator==(const _SmtfilmOpacities_t_ & other) const {
		return (memcmp(mOpacities, other.mOpacities, sizeof(int) * eSmtfilePos_MAX) == 0);
	}

} SmtfilmOpacities_t;



/*
	Wiper DataTypes
*/

// WiperOpMode_e
#define ENUM_LIST_WIPER_OPMODE \
	X(eWiperOpMode_Off, 0) \
	X(eWiperOpMode_Mist, 1) \
	X(eWiperOpMode_Low, 2) \
	X(eWiperOpMode_High, 3) \
	X(eWiperOpMode_Int1, 4) \
	X(eWiperOpMode_Int2, 5) \
	X(eWiperOpMode_Int3, 6) \
	X(eWiperOpMode_Int4, 7) \
	X(eWiperOpMode_Int5, 8)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_WIPER_OPMODE
#undef X
} WiperOpMode_e;



/*
	MultiSwitch
*/

// MfswWiperSw_e
#define ENUM_LIST_MFSW_WIPER_SWITCH \
	X(eMfswWiperSw_Off, 0) \
	X(eMfswWiperSw_Int, 1) \
	X(eMfswWiperSw_Auto, 2) \
	X(eMfswWiperSw_Low, 3) \
	X(eMfswWiperSw_High, 4) \
	X(eMfswWiperSw_Error, 126) \
	X(eMfswWiperSw_Unknown, 127)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_MFSW_WIPER_SWITCH
#undef X
} MfswWiperSw_e;

// MfswWasherSw_e
#define ENUM_LIST_MFSW_WASHER_SWITCH \
	X(eMfswWasherSw_Off, 0) \
	X(eMfswWasherSw_On, 1) \
	X(eMfswWasherSw_Error, 126) \
	X(eMfswWasherSw_Unknown, 127)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_MFSW_WASHER_SWITCH
#undef X
} MfswWasherSw_e;

// MfswWiperIntStepSw_e
#define ENUM_LIST_MFSW_WIPER_INTSTEP_SWITCH \
	X(eMfswWiperIntStepSw_1, 1) \
	X(eMfswWiperIntStepSw_2, 2) \
	X(eMfswWiperIntStepSw_3, 3) \
	X(eMfswWiperIntStepSw_4, 4) \
	X(eMfswWiperIntStepSw_5, 5) \
	X(eMfswWiperIntStepSw_Error, 126) \
	X(eMfswWiperIntStepSw_Unknown, 127)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_MFSW_WIPER_INTSTEP_SWITCH
#undef X
} MfswWiperIntStepSw_e;

// MfswWiperMistSw_e
#define ENUM_LIST_MFSW_WIPER_MIST_SWITCH \
	X(eMfswWiperMistSw_Off, 0) \
	X(eMfswWiperMistSw_On, 1) \
	X(eMfswWiperMistSw_Error, 126) \
	X(eMfswWiperMistSw_Unknown, 127)

typedef enum {
#define X(name,value) name=value,
	ENUM_LIST_MFSW_WIPER_MIST_SWITCH
#undef X
} MfswWiperMistSw_e;

#endif // __SOA_DATA_TYPE_H__
