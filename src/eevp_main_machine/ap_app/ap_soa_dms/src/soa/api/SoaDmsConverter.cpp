#include <Common.h>
#include <SoaDataType.h>
#include <SoaDmsConverter.hpp>

// DeviceNormal
eevp::control::SoaDeviceIsNormal Convert_DeviceNormal_SOA2API(DeviceNormal_e isNormal)
{
	switch (isNormal)
	{
	case eDeviceNormal_Ok:
		return eevp::control::SoaDeviceIsNormal::kNORMAL;
	case eDeviceNormal_Abnormal:
		return eevp::control::SoaDeviceIsNormal::kABNORMAL;
	default:
		break;
	}

	return eevp::control::SoaDeviceIsNormal::kABNORMAL;
}

DeviceNormal_e Convert_DeviceNormal_API2SOA(eevp::control::SoaDeviceIsNormal isNormal)
{
	switch (isNormal)
	{
	case eevp::control::SoaDeviceIsNormal::kNORMAL:
		return eDeviceNormal_Ok;
	case eevp::control::SoaDeviceIsNormal::kABNORMAL:
		return eDeviceNormal_Abnormal;
	default:
		break;
	}

	return eDeviceNormal_Abnormal;
}

// DriverInside
eevp::control::SoaDmsDriverInside Convert_DriverInside_SOA2API(DmsDriverInside_e inside)
{
	switch (inside)
	{
	case eDmsDriverInside_Unknown:
		return eevp::control::SoaDmsDriverInside::kUNKNOWN;
	case eDmsDriverInside_DriverIn:
		return eevp::control::SoaDmsDriverInside::kDRIVER;
	case eDmsDriverInside_None:
		return eevp::control::SoaDmsDriverInside::kNONE;
	default:
		break;
	}

	return eevp::control::SoaDmsDriverInside::kUNKNOWN;
}

DmsDriverInside_e Convert_DriverInside_API2SOA(eevp::control::SoaDmsDriverInside inside)
{
	switch (inside)
	{
	case eevp::control::SoaDmsDriverInside::kUNKNOWN:
		return eDmsDriverInside_Unknown;
	case eevp::control::SoaDmsDriverInside::kDRIVER:
		return eDmsDriverInside_DriverIn;
	case eevp::control::SoaDmsDriverInside::kNONE:
		return eDmsDriverInside_None;
	default:
		break;
	}

	return eDmsDriverInside_Unknown;
}

// Fatigue
eevp::control::SoaDmsFatigue Convert_Fatigue_SOA2API(DmsFatigue_e fatigue)
{
	switch (fatigue)
	{
	case eDmsFatigue_Unknown:
		return eevp::control::SoaDmsFatigue::kUNKNOWN;
	case eDmsFatigue_Ok:
		return eevp::control::SoaDmsFatigue::kOK;
	case eDmsFatigue_Drowsy:
		return eevp::control::SoaDmsFatigue::kDROWSY;
	case eDmsFatigue_Sleep:
		return eevp::control::SoaDmsFatigue::kSLEEP;
	default:
		break;
	}

	return eevp::control::SoaDmsFatigue::kUNKNOWN;
}

DmsFatigue_e Convert_Fatigue_API2SOA(eevp::control::SoaDmsFatigue fatigue)
{
	switch (fatigue)
	{
	case eevp::control::SoaDmsFatigue::kUNKNOWN:
		return eDmsFatigue_Unknown;
	case eevp::control::SoaDmsFatigue::kOK:
		return eDmsFatigue_Ok;
	case eevp::control::SoaDmsFatigue::kDROWSY:
		return eDmsFatigue_Drowsy;
	case eevp::control::SoaDmsFatigue::kSLEEP:
		return eDmsFatigue_Sleep;
	default:
		break;
	}

	return eDmsFatigue_Unknown;
}

// GazingDir
eevp::control::SoaDmsGazingDir Convert_GazingDir_SOA2API(DmsGazingDir_e dir)
{
	switch (dir)
	{
	case eDmsGazingDir_Unknown:
		return eevp::control::SoaDmsGazingDir::kUNKNOWN;
	case eDmsGazingDir_Front:
		return eevp::control::SoaDmsGazingDir::kFRONT;
	case eDmsGazingDir_FrontLeft:
		return eevp::control::SoaDmsGazingDir::kFRONT_LEFT;
	case eDmsGazingDir_FrontRight:
		return eevp::control::SoaDmsGazingDir::kFRONT_RIGHT;
	case eDmsGazingDir_RearMirror:
		return eevp::control::SoaDmsGazingDir::kREAR_MIRROR;
	case eDmsGazingDir_LeftMirror:
		return eevp::control::SoaDmsGazingDir::kLEFT_MIRROR;
	case eDmsGazingDir_RightMirror:
		return eevp::control::SoaDmsGazingDir::kRIGHT_MIRROR;
	case eDmsGazingDir_DashBoard:
		return eevp::control::SoaDmsGazingDir::kDASH_BOARD;
	case eDmsGazingDir_Media:
		return eevp::control::SoaDmsGazingDir::kMEDIA;
	case eDmsGazingDir_Other:
		return eevp::control::SoaDmsGazingDir::kOTHER;
	default:
		break;
	}

	return eevp::control::SoaDmsGazingDir::kUNKNOWN;
}

DmsGazingDir_e Convert_GazingDir_API2SOA(eevp::control::SoaDmsGazingDir dir)
{
	switch (dir)
	{
	case eevp::control::SoaDmsGazingDir::kUNKNOWN:
		return eDmsGazingDir_Unknown;
	case eevp::control::SoaDmsGazingDir::kFRONT:
		return eDmsGazingDir_Front;
	case eevp::control::SoaDmsGazingDir::kFRONT_LEFT:
		return eDmsGazingDir_FrontLeft;
	case eevp::control::SoaDmsGazingDir::kFRONT_RIGHT:
		return eDmsGazingDir_FrontRight;
	case eevp::control::SoaDmsGazingDir::kREAR_MIRROR:
		return eDmsGazingDir_RearMirror;
	case eevp::control::SoaDmsGazingDir::kLEFT_MIRROR:
		return eDmsGazingDir_LeftMirror;
	case eevp::control::SoaDmsGazingDir::kRIGHT_MIRROR:
		return eDmsGazingDir_RightMirror;
	case eevp::control::SoaDmsGazingDir::kDASH_BOARD:
		return eDmsGazingDir_DashBoard;
	case eevp::control::SoaDmsGazingDir::kMEDIA:
		return eDmsGazingDir_Media;
	case eevp::control::SoaDmsGazingDir::kOTHER:
		return eDmsGazingDir_Other;
	default:
		break;
	}

	return eDmsGazingDir_Unknown;
}

// OnPhone
eevp::control::SoaDmsOnPhone Convert_OnPhone_SOA2API(DmsOnPhone_e phone)
{
	switch (phone)
	{
	case eDmsOnPhone_Unknown:
		return eevp::control::SoaDmsOnPhone::kUNKNOWN;
	case eDmsOnPhone_OnPhone:
		return eevp::control::SoaDmsOnPhone::kON_PHONE;
	case eDmsOnPhone_NotOnPhone:
		return eevp::control::SoaDmsOnPhone::kNOT_ON_PHONE;
	default:
		break;
	}

	return eevp::control::SoaDmsOnPhone::kUNKNOWN;
}

DmsOnPhone_e Convert_OnPhone_API2SOA(eevp::control::SoaDmsOnPhone phone)
{
	switch (phone)
	{
	case eevp::control::SoaDmsOnPhone::kUNKNOWN:
		return eDmsOnPhone_Unknown;
	case eevp::control::SoaDmsOnPhone::kON_PHONE:
		return eDmsOnPhone_OnPhone;
	case eevp::control::SoaDmsOnPhone::kNOT_ON_PHONE:
		return eDmsOnPhone_NotOnPhone;
	default:
		break;
	}

	return eDmsOnPhone_Unknown;
}

// Smoke
eevp::control::SoaDmsSmoking Convert_Smoke_SOA2API(DmsSmoking_e smoke)
{
	switch (smoke)
	{
	case eDmsSmoking_Unknown:
		return eevp::control::SoaDmsSmoking::kUNKNOWN;
	case eDmsSmoking_Smoking:
		return eevp::control::SoaDmsSmoking::kSMOKE;
	case eDmsSmoking_NoSmoking:
		return eevp::control::SoaDmsSmoking::kNOSMOKE;
	default:
		break;
	}

	return eevp::control::SoaDmsSmoking::kUNKNOWN;
}

DmsSmoking_e Convert_Smoke_API2SOA(eevp::control::SoaDmsSmoking smoke)
{
	switch (smoke)
	{
	case eevp::control::SoaDmsSmoking::kUNKNOWN:
		return eDmsSmoking_Unknown;
	case eevp::control::SoaDmsSmoking::kSMOKE:
		return eDmsSmoking_Smoking;
	case eevp::control::SoaDmsSmoking::kNOSMOKE:
		return eDmsSmoking_NoSmoking;
	default:
		break;
	}

	return eDmsSmoking_Unknown;
}
