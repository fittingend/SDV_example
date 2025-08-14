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
    X(eDeviceNormal_Ok) \
    X(eDeviceNormal_Abnormal)

typedef enum {
    #define X(name) name,
    ENUM_LIST_DEVICE_NORMAL
    #undef X
} DeviceNormal_e;

// ClientLevel_e
#define ENUM_LIST_CLIENT_LEVEL \
    X(eClientLevel_SubscribedApps) \
    X(eClientLevel_GuiApps) \
    X(eClientLevel_PhysicalSwitch)

typedef enum {
    #define X(name) name,
    ENUM_LIST_CLIENT_LEVEL
    #undef X
} ClientLevel_e;

// PowerState_e
#define ENUM_LIST_POWER_STATE \
    X(ePowerState_Off) \
    X(ePowerState_On) \
    X(ePowerState_Error)

typedef enum {
    #define X(name) name,
    ENUM_LIST_POWER_STATE
    #undef X
} PowerState_e;

/*
    DMS DataTypes
*/

// DmsDriverInside_e
#define ENUM_LIST_DMS_DRIVER_INSIDE \
    X(eDmsDriverInside_Unknown) \
    X(eDmsDriverInside_DriverIn) \
    X(eDmsDriverInside_None)

typedef enum {
    #define X(name) name,
    ENUM_LIST_DMS_DRIVER_INSIDE
    #undef X
} DmsDriverInside_e;

// DmsFatigue_e
#define ENUM_LIST_DMS_FATIGUE \
    X(eDmsFatigue_Unknown) \
    X(eDmsFatigue_Ok) \
    X(eDmsFatigue_Drowsy) \
    X(eDmsFatigue_Sleep)

typedef enum {
    #define X(name) name,
    ENUM_LIST_DMS_FATIGUE
    #undef X
} DmsFatigue_e;

// DmsGazingDir_e
#define ENUM_LIST_DMS_GAZING_DIR \
    X(eDmsGazingDir_Unknown) \
    X(eDmsGazingDir_Front) \
    X(eDmsGazingDir_FrontLeft) \
    X(eDmsGazingDir_FrontRight) \
    X(eDmsGazingDir_RearMirror) \
    X(eDmsGazingDir_LeftMirror) \
    X(eDmsGazingDir_RightMirror) \
    X(eDmsGazingDir_DashBoard) \
    X(eDmsGazingDir_Media) \
    X(eDmsGazingDir_Other)

typedef enum {
    #define X(name) name,
    ENUM_LIST_DMS_GAZING_DIR
    #undef X
} DmsGazingDir_e;

// DmsOnPhone_e
#define ENUM_LIST_DMS_ON_PHONE \
    X(eDmsOnPhone_Unknown) \
    X(eDmsOnPhone_OnPhone) \
    X(eDmsOnPhone_NotOnPhone)

typedef enum {
    #define X(name) name,
    ENUM_LIST_DMS_ON_PHONE
    #undef X
} DmsOnPhone_e;

// DmsSmoking_e
#define ENUM_LIST_DMS_SMOKING \
    X(eDmsSmoking_Unknown) \
    X(eDmsSmoking_Smoking) \
    X(eDmsSmoking_NoSmoking)

typedef enum {
    #define X(name) name,
    ENUM_LIST_DMS_SMOKING
    #undef X
} DmsSmoking_e;

/*
    HVAC DataTypes
*/

// HvacAirSource_e
#define ENUM_LIST_HVAC_AIRSOURCE \
    X(eHvacAirSource_Outer) \
    X(eHvacAirSource_Inner)

typedef enum {
    #define X(name) name,
    ENUM_LIST_HVAC_AIRSOURCE
    #undef X
} HvacAirSource_e;

// HvacBlowDir_e
#define ENUM_LIST_HVAC_BLOWDIR \
    X(eHvacBlowDir_Chest) \
    X(eHvacBlowDir_Both) \
    X(eHvacBlowDir_Foot) \
    X(eHvacBlowDir_FootWithWS)

typedef enum {
    #define X(name) name,
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

#endif // __SOA_DATA_TYPE_H__
