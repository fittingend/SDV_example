#include <Common.h>
#include <SoaDataType.h>
#include <Zone3RcurtainConverter.hpp>

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

// MotorDir
uint8_t Convert_MotorDir_SOA2Zone(RCtnMotorDir_e dir)
{
	switch (dir)
	{
	case eRCtnMotorDir_Off:
		return 0;
	case eRCtnMotorDir_OpenOn:
		return 4;
	case eRCtnMotorDir_CloseOn:
		return 1;
	default:
		break;
	}

	return 0;
}

RCtnMotorDir_e Convert_MotorDir_Zone2SOA(uint8_t dir)
{
	switch (dir)
	{
	case 0:
		return eRCtnMotorDir_Off;
	case 4:
		return eRCtnMotorDir_OpenOn;
	case 1:
		return eRCtnMotorDir_CloseOn;
	default:
		break;
	}

	return eRCtnMotorDir_Off;
}

// Position
uint8_t Convert_Position_SOA2Zone(RCtnPos_e pos)
{
	switch (pos)
	{
	case eRCtnPos_FullyClosed:
		return 1;
	case eRCtnPos_Closed66:
		return 2;
	case eRCtnPos_Closed33:
		return 3;
	case eRCtnPos_FullyOpened:
		return 4;
	default:
		break;
	}

	return 0;
}

RCtnPos_e Convert_Position_Zone2SOA(uint8_t pos)
{
	switch (pos)
	{
	case 1:
		return eRCtnPos_FullyClosed;
	case 2:
		return eRCtnPos_Closed66;
	case 3:
		return eRCtnPos_Closed33;
	case 4:
		return eRCtnPos_FullyOpened;
	default:
		break;
	}

	return eRCtnPos_Unknown;
}

