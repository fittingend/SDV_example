#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>
#include <string.h>
#include <HvacDataTypeConverter.h>
#include "eevp/control/impl_type_soadeviceisnormal.h"
#include "eevp/control/impl_type_soahvacairsource.h"
#include "eevp/control/impl_type_soahvacblowdir.h"
#include "eevp/control/impl_type_soahvacstatus.h"
#include "eevp/control/impl_type_soahvactemps.h"

// AirSource
eevp::control::SoaHvacAirSource Convert_HvacAirSource_SOA2API(HvacAirSource_e airSrc)
{
    switch (airSrc)
    {
    case eHvacAirSource_Outer:
        return eevp::control::SoaHvacAirSource::kOUTER;
    case eHvacAirSource_Inner:
        return eevp::control::SoaHvacAirSource::kINNER;
    default:
        break;
    }

    return eevp::control::SoaHvacAirSource::kINNER;
}

HvacAirSource_e Convert_HvacAirSource_API2SOA(eevp::control::SoaHvacAirSource airSrc)
{
    switch (airSrc)
    {
    case eevp::control::SoaHvacAirSource::kOUTER:
        return eHvacAirSource_Outer;
    case eevp::control::SoaHvacAirSource::kINNER:
        return eHvacAirSource_Inner;
    default:
        break;
    }

    return eHvacAirSource_Inner;
}

// BlowDir
eevp::control::SoaHvacBlowDir Convert_HvacBlowDir_SOA2API(HvacBlowDir_e blowDir)
{
    switch (blowDir)
    {
    case eHvacBlowDir_Chest:
        return eevp::control::SoaHvacBlowDir::kCHEST;
    case eHvacBlowDir_Both:
        return eevp::control::SoaHvacBlowDir::kBOTH;
    case eHvacBlowDir_Foot:
        return eevp::control::SoaHvacBlowDir::kFOOT;
    case eHvacBlowDir_FootWithWS:
        return eevp::control::SoaHvacBlowDir::kFOOT_WITH_WS;
    default:
        break;
    }

    return eevp::control::SoaHvacBlowDir::kCHEST;
}

HvacBlowDir_e Convert_HvacBlowDir_API2SOA(eevp::control::SoaHvacBlowDir blowDir)
{
    switch (blowDir)
    {
    case eevp::control::SoaHvacBlowDir::kCHEST:
        return eHvacBlowDir_Chest;
    case eevp::control::SoaHvacBlowDir::kBOTH:
        return eHvacBlowDir_Both;
    case eevp::control::SoaHvacBlowDir::kFOOT:
        return eHvacBlowDir_Foot;
    case eevp::control::SoaHvacBlowDir::kFOOT_WITH_WS:
        return eHvacBlowDir_FootWithWS;
    default:
        break;
    }

    return eHvacBlowDir_Chest;
}

// HVAC Status
void Convert_HvacStatus_SOA2API(const HvacStatus_t &src, eevp::control::SoaHvacStatus &dst)
{
    dst.powerOn = src.mPowerOn;
    dst.acOnOff = src.mAcOnOff;
    dst.autoMode = src.mAutoMode;
    dst.syncMode = src.mSyncMode;
    dst.driverTemp = (std::int8_t)src.mDriverTemp;
    dst.passengerTemp = (std::int8_t)src.mPassengerTemp;
    dst.blowingForce = (std::uint8_t)src.mBlowingForce;
    dst.blowingDir = Convert_HvacBlowDir_SOA2API(src.mBlowDir);
    dst.airSrc = Convert_HvacAirSource_SOA2API(src.mAirSrc);
    dst.forwardFrostOn = src.mForwardFrostOn;
    dst.backwardFrostOn = src.mBackwardFrostOn;
}

void Convert_HvacStatus_API2SOA(const eevp::control::SoaHvacStatus &src, HvacStatus_t &dst)
{
    dst.mPowerOn = src.powerOn;
    dst.mAcOnOff = src.acOnOff;
    dst.mAutoMode = src.autoMode;
    dst.mSyncMode = src.syncMode;
    dst.mDriverTemp = (int)src.driverTemp;
    dst.mPassengerTemp = (int)src.passengerTemp;
    dst.mBlowingForce = (int)src.blowingForce;
    dst.mBlowDir = Convert_HvacBlowDir_API2SOA(src.blowingDir);
    dst.mAirSrc = Convert_HvacAirSource_API2SOA(src.airSrc);
    dst.mForwardFrostOn = src.forwardFrostOn;
    dst.mBackwardFrostOn = src.backwardFrostOn;
}

extern bool CompareAndUpdate_HvacStatus_API(const eevp::control::SoaHvacStatus &src, eevp::control::SoaHvacStatus &dst, bool update)
{
    bool isUpdated = false;
    if ((dst.powerOn != src.powerOn) || (dst.acOnOff != src.acOnOff) || (dst.autoMode != src.autoMode) || (dst.syncMode != src.syncMode) \
        || (dst.driverTemp != src.driverTemp) || (dst.passengerTemp != src.passengerTemp) || (dst.blowingForce != src.blowingForce) \
        || (dst.blowingDir != src.blowingDir) || (dst.airSrc != src.airSrc) \
        || (dst.forwardFrostOn != src.forwardFrostOn) || (dst.backwardFrostOn != src.backwardFrostOn))

    {
        isUpdated = true;
    }

    if (update && isUpdated)
    {
        memcpy(&dst, &src, sizeof(eevp::control::SoaHvacStatus));
    }

    return isUpdated;
}

// HVAC Temps
void Convert_HvacTemps_SOA2API(const HvacTemps_t &src, eevp::control::SoaHvacTemps &dst)
{
    dst.inside = (std::int8_t)src.mInside;
    dst.outside = (std::int8_t)src.mOutside;
    dst.duct = (std::int8_t)src.mDuct;
    dst.evaporator = (std::int8_t)src.mEvaporator;
    dst.driverVentDuct = (std::int8_t)src.mDriverVentDuct;
    dst.driverFloorDuct = (std::int8_t)src.mDriverFloorDuct;
    dst.passengerVentDuct = (std::int8_t)src.mPassengerVentDuct;
    dst.passengerFloorDuct = (std::int8_t)src.mPassengerFloorDuct;
}

void Convert_HvacTemps_API2SOA(const eevp::control::SoaHvacTemps &src, HvacTemps_t &dst)
{
    dst.mInside = (int)src.inside;
    dst.mOutside = (int)src.outside;
    dst.mDuct = (int)src.duct;
    dst.mEvaporator = (int)src.evaporator;
    dst.mDriverVentDuct = (int)src.driverVentDuct;
    dst.mDriverFloorDuct = (int)src.driverFloorDuct;
    dst.mPassengerVentDuct = (int)src.passengerVentDuct;
    dst.mPassengerFloorDuct = (int)src.passengerFloorDuct;
}

extern bool CompareAndUpdate_HvacTemps_API(const eevp::control::SoaHvacTemps &src, eevp::control::SoaHvacTemps &dst, bool update)
{
    bool isUpdated = false;
    if ((dst.inside != src.inside) || (dst.outside != src.outside) || (dst.duct != src.duct) || (dst.evaporator != src.evaporator) \
        || (dst.driverVentDuct != src.driverVentDuct) || (dst.driverFloorDuct != src.driverFloorDuct) \
        || (dst.passengerVentDuct != src.passengerVentDuct) || (dst.passengerFloorDuct != src.passengerFloorDuct))

    {
        isUpdated = true;
    }

    if (update && isUpdated)
    {
        memcpy(&dst, &src, sizeof(eevp::control::SoaHvacTemps));
    }

    return isUpdated;
}
