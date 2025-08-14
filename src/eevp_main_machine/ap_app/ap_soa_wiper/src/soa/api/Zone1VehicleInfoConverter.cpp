#include <Common.h>
#include <SoaDataType.h>
#include <Zone1VehicleInfoConverter.hpp>

// GearState_e
std::uint8_t Convert_GearState_SOA2Zone(GearState_e gear)
{
	switch (gear)
	{
	case eGearState_P:
		return 0x00;
	case eGearState_D:
		return 0x05;
	case eGearState_N:
		return 0x06;
	case eGearState_R:
		return 0x07;
	case eGearState_L:
		return 0x01;
	case eGearState_2:
		return 0x02;
	case eGearState_3:
		return 0x03;
	case eGearState_DS:
		return 0x04;
	case eGearState_ManualShift:
		return 0x08;
	case eGearState_PaddleShift:
		return 0x0C;
	case eGearState_SubcomComm:
		return 0x0A;
	case eGearState_Unknown:
		return 0x0F;
	default:
		break;
	}

	return 0x0F;
}

GearState_e Convert_GearState_Zone2SOA(std::uint8_t gear)
{
	switch (gear)
	{
	case 0x00:
		return eGearState_P;
	case 0x05:
		return eGearState_D;
	case 0x06:
		return eGearState_N;
	case 0x07:
		return eGearState_R;
	case 0x01:
		return eGearState_L;
	case 0x02:
		return eGearState_2;
	case 0x03:
		return eGearState_3;
	case 0x04:
		return eGearState_DS;
	case 0x08:
		return eGearState_ManualShift;
	case 0x0C:
		return eGearState_PaddleShift;
	case 0x0A:
		return eGearState_SubcomComm;
	default:
		break;
	}

	return eGearState_Fault;
}

// IgnState_e
std::uint8_t Convert_IgnState_SOA2Zone(IgnState_e ign)
{
	switch (ign)
	{
	case eIgnState_Off:
		return 0x00;
	case eIgnState_On:
		return 0x01;
	case eIgnState_Error:
		return 0x03;
	default:
		break;
	}

	return 0x03;
}

IgnState_e Convert_IgnState_Zone2SOA(std::uint8_t ign)
{
	switch (ign)
	{
	case 0x00:
		return eIgnState_Off;
	case 0x01:
		return eIgnState_On;
	case 0x03:
		return eIgnState_Error;
	default:
		break;
	}

	return eIgnState_Unknown;
}

// IgnState_e
std::uint8_t Convert_AccState_SOA2Zone(AccState_e acc)
{
	switch (acc)
	{
	case eAccState_Off:
		return 0x00;
	case eAccState_On:
		return 0x01;
	case eAccState_Error:
		return 0x03;
	default:
		break;
	}

	return 0x03;
}

AccState_e Convert_AccState_Zone2SOA(std::uint8_t acc)
{
	switch (acc)
	{
	case 0x00:
		return eAccState_Off;
	case 0x01:
		return eAccState_On;
	case 0x03:
		return eAccState_Error;
	default:
		break;
	}

	return eAccState_Unknown;
}
