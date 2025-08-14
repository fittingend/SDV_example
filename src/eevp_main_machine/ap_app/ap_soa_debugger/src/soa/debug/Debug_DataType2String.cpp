#include <Common.h>
#include <SoaDataType.h>
#include <string>
#include <sstream>
#include <Debug_DataType2String.hpp>

std::string getStringEnum_bool(bool val)
{
    return (val) ? "true" : "false";
}

std::string getStringEnum_DeviceNormal_e(DeviceNormal_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_DEVICE_NORMAL
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_ClientLevel_e(ClientLevel_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_CLIENT_LEVEL
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_PowerState_e(PowerState_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_POWER_STATE
    #undef X
    default:
        break;
    }

    return "N/A";
}

/*
    DMS
*/

std::string getStringEnum_DmsDriverInside_e(DmsDriverInside_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_DMS_DRIVER_INSIDE
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_DmsFatigue_e(DmsFatigue_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_DMS_FATIGUE
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_DmsGazingDir_e(DmsGazingDir_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_DMS_GAZING_DIR
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_DmsOnPhone_e(DmsOnPhone_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_DMS_ON_PHONE
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_DmsSmoking_e(DmsSmoking_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_DMS_SMOKING
    #undef X
    default:
        break;
    }

    return "N/A";
}

/*
    HVAC
*/
std::string getStringEnum_HvacAirSource_e(HvacAirSource_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
    ENUM_LIST_HVAC_AIRSOURCE
    #undef X
    default:
        break;
    }

    return "N/A";
}

std::string getStringEnum_HvacBlowDir_e(HvacBlowDir_e val)
{
    switch (val)
    {
    #define X(name) case name: return #name;
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
