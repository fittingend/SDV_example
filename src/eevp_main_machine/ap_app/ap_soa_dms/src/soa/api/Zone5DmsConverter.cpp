#include <Common.h>
#include <SoaDataType.h>
#include <Zone5DmsConverter.hpp>

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

// DriverInside
uint8_t Convert_DriverInside_SOA2Zone(DmsDriverInside_e inside)
{
	switch (inside)
	{
	case eDmsDriverInside_Unknown:
		return 0;
	case eDmsDriverInside_DriverIn:
		return 1;
	case eDmsDriverInside_None:
		return 2;
	default:
		break;
	}

	return 0;
}

DmsDriverInside_e Convert_DriverInside_Zone2SOA(uint8_t inside)
{
	switch (inside)
	{
	case 0:
		return eDmsDriverInside_Unknown;
	case 1:
		return eDmsDriverInside_DriverIn;
	case 2:
		return eDmsDriverInside_None;
	default:
		break;
	}

	return eDmsDriverInside_Unknown;
}

// Fatigue
uint8_t Convert_Fatigue_SOA2Zone(DmsFatigue_e fatigue)
{
	switch (fatigue)
	{
	case eDmsFatigue_Unknown:
		return 0;
	case eDmsFatigue_Ok:
		return 1;
	case eDmsFatigue_Drowsy:
		return 2;
	case eDmsFatigue_Sleep:
		return 3;
	default:
		break;
	}

	return 0;
}

DmsFatigue_e Convert_Fatigue_Zone2SOA(uint8_t fatigue)
{
	switch (fatigue)
	{
	case 0:
		return eDmsFatigue_Unknown;
	case 1:
		return eDmsFatigue_Ok;
	case 2:
		return eDmsFatigue_Drowsy;
	case 3:
		return eDmsFatigue_Sleep;
	default:
		break;
	}

	return eDmsFatigue_Unknown;
}

// GazingDir
uint8_t Convert_GazingDir_SOA2Zone(DmsGazingDir_e dir)
{
	switch (dir)
	{
	case eDmsGazingDir_Unknown:
		return 0;
	case eDmsGazingDir_Front:
		return 1;
	case eDmsGazingDir_FrontLeft:
		return 2;
	case eDmsGazingDir_FrontRight:
		return 3;
	case eDmsGazingDir_RearMirror:
		return 4;
	case eDmsGazingDir_LeftMirror:
		return 5;
	case eDmsGazingDir_RightMirror:
		return 6;
	case eDmsGazingDir_DashBoard:
		return 7;
	case eDmsGazingDir_Media:
		return 8;
	case eDmsGazingDir_Other:
		return 9;
	default:
		break;
	}

	return 0;
}

DmsGazingDir_e Convert_GazingDir_Zone2SOA(uint8_t dir)
{
	switch (dir)
	{
	case 0:
		return eDmsGazingDir_Unknown;
	case 1:
		return eDmsGazingDir_Front;
	case 2:
		return eDmsGazingDir_FrontLeft;
	case 3:
		return eDmsGazingDir_FrontRight;
	case 4:
		return eDmsGazingDir_RearMirror;
	case 5:
		return eDmsGazingDir_LeftMirror;
	case 6:
		return eDmsGazingDir_RightMirror;
	case 7:
		return eDmsGazingDir_DashBoard;
	case 8:
		return eDmsGazingDir_Media;
	case 9:
		return eDmsGazingDir_Other;
	default:
		break;
	}

	return eDmsGazingDir_Unknown;
}

// OnPhone
uint8_t Convert_OnPhone_SOA2Zone(DmsOnPhone_e phone)
{
	switch (phone)
	{
	case eDmsOnPhone_Unknown:
		return 0;
	case eDmsOnPhone_OnPhone:
		return 1;
	case eDmsOnPhone_NotOnPhone:
		return 2;
	default:
		break;
	}

	return 0;
}

DmsOnPhone_e Convert_OnPhone_Zone2SOA(uint8_t phone)
{
	switch (phone)
	{
	case 0:
		return eDmsOnPhone_Unknown;
	case 1:
		return eDmsOnPhone_OnPhone;
	case 2:
		return eDmsOnPhone_NotOnPhone;
	default:
		break;
	}

	return eDmsOnPhone_Unknown;
}

// Smoke
uint8_t Convert_Smoke_SOA2Zone(DmsSmoking_e smoke)
{
	switch (smoke)
	{
	case eDmsSmoking_Unknown:
		return 0;
	case eDmsSmoking_Smoking:
		return 1;
	case eDmsSmoking_NoSmoking:
		return 2;
	default:
		break;
	}

	return 0;
}

DmsSmoking_e Convert_Smoke_Zone2SOA(uint8_t smoke)
{
	switch (smoke)
	{
	case 0:
		return eDmsSmoking_Unknown;
	case 1:
		return eDmsSmoking_Smoking;
	case 2:
		return eDmsSmoking_NoSmoking;
	default:
		break;
	}

	return eDmsSmoking_Unknown;
}
