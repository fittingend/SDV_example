#include <Common.h>
#include <SoaDataType.h>
#include <cstdint>
#include "eevp/control/impl_type_soafilmopacityarray.h"

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

// SmartFilm Position
uint8_t Convert_SmtfilmPos_SOA2Zone(SmtfilmPos_e pos)
{
	switch (pos)
	{
	case eSmtfilmPos_WindShield:
		return 1;
	case eSmtfilmPos_RoofGlass:
		return 2;
	case eSmtfilmPos_DsFrontRear:
		return 3;
	case eSmtfilmPos_DsQuarter:
		return 4;
	case eSmtfilmPos_AsFrontRear:
		return 5;
	case eSmtfilmPos_AsQuarter:
		return 6;
	case eSmtfilmPos_RearGrass:
		return 7;
	default:
		break;
	}

	return 1;
}

SmtfilmPos_e Convert_SmtfilmPos_Zone2SOA(uint8_t pos)
{	switch (pos)
	{
	case 1:
		return eSmtfilmPos_WindShield;
	case 2:
		return eSmtfilmPos_RoofGlass;
	case 3:
		return eSmtfilmPos_DsFrontRear;
	case 4:
		return eSmtfilmPos_DsQuarter;
	case 5:
		return eSmtfilmPos_AsFrontRear;
	case 6:
		return eSmtfilmPos_AsQuarter;
	case 7:
		return eSmtfilmPos_RearGrass;

	default:
		break;
	}

	return eSmtfilmPos_WindShield;
}

// SmartFilm Opacities
void Convert_SmtfilmOpacity_SOA2Zone(const SmtfilmOpacities_t &src, eevp::control::SoaFilmOpacityArray &dst)
{
	for (int i = 0; i < dst.size(); i++)
	{
		dst[i] = (i < eSmtfilePos_MAX) ? (std::uint8_t)src.mOpacities[i] : (std::uint8_t)0;
	}
}

void Convert_SmtfilmOpacity_Zone2SOA(const eevp::control::SoaFilmOpacityArray &src, SmtfilmOpacities_t &dst)
{
	for (int i = 0; i < eSmtfilePos_MAX; i++)
	{
		dst.mOpacities[i] = (i < src.size()) ? (int)src[i] : (int)0;
	}
}