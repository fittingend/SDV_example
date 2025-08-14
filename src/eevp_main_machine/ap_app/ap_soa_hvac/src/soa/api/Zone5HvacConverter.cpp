#include <Common.h>
#include <SoaDataType.h>
#include <Zone5HvacConverter.hpp>
#include <zone5/hvac/control/impl_type_ecuz5hvacstatus.h>
#include <zone5/hvac/control/impl_type_ecuz5hvactemps.h>

// DeviceNormal
uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal)
{
	switch (isNormal)
	{
	case eDeviceNormal_Ok:
		return 0;
	case eDeviceNormal_Abnormal:
		return 1;
	default:
		break;
	}

	return 1;
}

DeviceNormal_e Convert_DeviceNormal_Zone2SOA(uint8_t isNormal)
{
	switch (isNormal)
	{
	case 0:
		return eDeviceNormal_Ok;
	case 1:
		return eDeviceNormal_Abnormal;
	default:
		break;
	}

	return eDeviceNormal_Abnormal;
}

// AirSource
uint8_t Convert_AirSource_SOA2Zone(HvacAirSource_e airSrc)
{
	switch (airSrc)
	{
	case eHvacAirSource_Outer:
		return 0;
	case eHvacAirSource_Inner:
		return 1;
	default:
		break;
	}

	return 1;
}

HvacAirSource_e Convert_AirSource_Zone2SOA(uint8_t airSrc)
{
	switch (airSrc)
	{
	case 0:
		return eHvacAirSource_Outer;
	case 1:
		return eHvacAirSource_Inner;
	default:
		break;
	}

	return eHvacAirSource_Inner;
}

// BlowDir
uint8_t Convert_BlowDir_SOA2Zone(HvacBlowDir_e blowDir)
{
	switch (blowDir)
	{
	case eHvacBlowDir_Chest:
		return 0;
	case eHvacBlowDir_Both:
		return 1;
	case eHvacBlowDir_Foot:
		return 2;
	case eHvacBlowDir_FootWithWS:
		return 3;
	default:
		break;
	}

	return 0;
}

HvacBlowDir_e Convert_BlowDir_Zone2SOA(uint8_t blowDir)
{
	switch (blowDir)
	{
	case 0:
		return eHvacBlowDir_Chest;
	case 1:
		return eHvacBlowDir_Both;
	case 2:
		return eHvacBlowDir_Foot;
	case 3:
		return eHvacBlowDir_FootWithWS;
	default:
		break;
	}

	return eHvacBlowDir_Chest;
}

// Status
void Convert_HvacStatus_SOA2Zone(const HvacStatus_t &src, zone5::hvac::control::EcuZ5HvacStatus &dst)
{
	dst.powerOn = (std::uint8_t)((src.mPowerOn) ? 1 : 0);
	dst.acOnOff = (std::uint8_t)((src.mAcOnOff) ? 1 : 0);
	dst.autoMode = (std::uint8_t)((src.mAutoMode) ? 1 : 0);
	dst.syncMode = (std::uint8_t)((src.mSyncMode) ? 1 : 0);
	dst.driverTemp = (std::int8_t)src.mDriverTemp;
	dst.passengerTemp = (std::int8_t)src.mPassengerTemp;
	dst.blowingForce = (std::uint8_t)src.mBlowingForce;
	dst.blowingDir = Convert_BlowDir_SOA2Zone(src.mBlowDir);
	dst.airSrc = Convert_AirSource_SOA2Zone(src.mAirSrc);
	dst.forwardDefrostOn = (std::uint8_t)((src.mForwardFrostOn) ? 1 : 0);
	dst.backwardDefrostOn = (std::uint8_t)((src.mBackwardFrostOn) ? 1 : 0);
}

void Convert_HvacStatus_Zone2SOA(const zone5::hvac::control::EcuZ5HvacStatus &src, HvacStatus_t &dst)
{
	dst.mPowerOn = (src.powerOn != 0);
	dst.mAcOnOff = (src.acOnOff != 0);
	dst.mAutoMode = (src.autoMode != 0);
	dst.mSyncMode = (src.syncMode != 0);
	dst.mDriverTemp = (int)src.driverTemp;
	dst.mPassengerTemp = (int)src.passengerTemp;
	dst.mBlowingForce = (int)src.blowingForce;
	dst.mBlowDir = Convert_BlowDir_Zone2SOA(src.blowingDir);
	dst.mAirSrc = Convert_AirSource_Zone2SOA(src.airSrc);
	dst.mForwardFrostOn = (src.forwardDefrostOn != 0);
	dst.mBackwardFrostOn = (src.backwardDefrostOn != 0);
}

// Temps
void Convert_HvacTemps_SOA2Zone(const HvacTemps_t &src, zone5::hvac::control::EcuZ5HvacTemps &dst)
{
	dst.inside = (std::int8_t)src.mInside;
	dst.outside = (std::int8_t)src.mOutside;
	dst.duct = (std::int8_t)src.mDuct;
	dst.evaporator = (std::int8_t)src.mEvaporator;
	dst.driverVentDuct = (std::int8_t)src.mDriverVentDuct;
	dst.driverFloarDuct = (std::int8_t)src.mDriverFloorDuct;
	dst.passengerVentDuct = (std::int8_t)src.mPassengerVentDuct;
	dst.passengerFloorDuct = (std::int8_t)src.mPassengerFloorDuct;
}

void Convert_HvacTemps_Zone2SOA(const zone5::hvac::control::EcuZ5HvacTemps &src, HvacTemps_t &dst)
{
	dst.mInside = (int)src.inside;
	dst.mOutside = (int)src.outside;
	dst.mDuct = (int)src.duct;
	dst.mEvaporator = (int)src.evaporator;
	dst.mDriverVentDuct = (int)src.driverVentDuct;
	dst.mDriverFloorDuct = (int)src.driverFloarDuct;
	dst.mPassengerVentDuct = (int)src.passengerVentDuct;
	dst.mPassengerFloorDuct = (int)src.passengerFloorDuct;
}
