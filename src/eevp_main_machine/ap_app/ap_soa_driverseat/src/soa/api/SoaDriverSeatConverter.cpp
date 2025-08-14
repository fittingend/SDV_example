#include <Common.h>
#include <SoaDataType.h>
#include <SoaDriverSeatConverter.hpp>

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

// SoaSeatHeightDirection
eevp::control::SoaSeatHeightDirection Convert_SeatHeightDir_SOA2API(DriverSeatDir_e heightDir)
{
	switch (heightDir)
	{
	case eDriverSeatDir_Stop:
		return eevp::control::SoaSeatHeightDirection::kSTOP;
	case eDriverSeatDir_Up:
		return eevp::control::SoaSeatHeightDirection::kUP;
	case eDriverSeatDir_Down:
		return eevp::control::SoaSeatHeightDirection::kDOWN;
	default:
		break;
	}

	return eevp::control::SoaSeatHeightDirection::kSTOP;
}

DriverSeatDir_e Convert_SeatHeightDir_API2SOA(eevp::control::SoaSeatHeightDirection heightDir)
{
	switch (heightDir)
	{
	case eevp::control::SoaSeatHeightDirection::kSTOP:
		return eDriverSeatDir_Stop;
	case eevp::control::SoaSeatHeightDirection::kUP:
		return eDriverSeatDir_Up;
	case eevp::control::SoaSeatHeightDirection::kDOWN:
		return eDriverSeatDir_Down;
	default:
		break;
	}

	return eDriverSeatDir_Stop;
}

// SoaSeatReclineDirection
eevp::control::SoaSeatReclineDirection Convert_SeatReclineDir_SOA2API(DriverSeatDir_e reclineDir)
{
	switch (reclineDir)
	{
	case eDriverSeatDir_Stop:
		return eevp::control::SoaSeatReclineDirection::kSTOP;
	case eDriverSeatDir_Forward:
		return eevp::control::SoaSeatReclineDirection::kFORWARD;
	case eDriverSeatDir_Backward:
		return eevp::control::SoaSeatReclineDirection::kBACKWARD;
	default:
		break;
	}

	return eevp::control::SoaSeatReclineDirection::kSTOP;
}

DriverSeatDir_e Convert_SeatReclineDir_API2SOA(eevp::control::SoaSeatReclineDirection reclineDir)
{
	switch (reclineDir)
	{
	case eevp::control::SoaSeatReclineDirection::kSTOP:
		return eDriverSeatDir_Stop;
	case eevp::control::SoaSeatReclineDirection::kFORWARD:
		return eDriverSeatDir_Forward;
	case eevp::control::SoaSeatReclineDirection::kBACKWARD:
		return eDriverSeatDir_Backward;
	default:
		break;
	}

	return eDriverSeatDir_Stop;
}

// SoaSeatSlideDirection
eevp::control::SoaSeatSlideDirection Convert_SeatSlideDir_SOA2API(DriverSeatDir_e slideDir)
{
	switch (slideDir)
	{
	case eDriverSeatDir_Stop:
		return eevp::control::SoaSeatSlideDirection::kSTOP;
	case eDriverSeatDir_Forward:
		return eevp::control::SoaSeatSlideDirection::kFORWARD;
	case eDriverSeatDir_Backward:
		return eevp::control::SoaSeatSlideDirection::kBACKWARD;
	default:
		break;
	}

	return eevp::control::SoaSeatSlideDirection::kSTOP;
}

DriverSeatDir_e Convert_SeatSlideDir_API2SOA(eevp::control::SoaSeatSlideDirection slideDir)
{
	switch (slideDir)
	{
	case eevp::control::SoaSeatSlideDirection::kSTOP:
		return eDriverSeatDir_Stop;
	case eevp::control::SoaSeatSlideDirection::kFORWARD:
		return eDriverSeatDir_Forward;
	case eevp::control::SoaSeatSlideDirection::kBACKWARD:
		return eDriverSeatDir_Backward;
	default:
		break;
	}

	return eDriverSeatDir_Stop;
}

// SoaSeatTiltDirection
eevp::control::SoaSeatTiltDirection Convert_SeatTiltDir_SOA2API(DriverSeatDir_e tiltDir)
{
	switch (tiltDir)
	{
	case eDriverSeatDir_Stop:
		return eevp::control::SoaSeatTiltDirection::kSTOP;
	case eDriverSeatDir_Up:
		return eevp::control::SoaSeatTiltDirection::kUP;
	case eDriverSeatDir_Down:
		return eevp::control::SoaSeatTiltDirection::kDOWN;
	default:
		break;
	}

	return eevp::control::SoaSeatTiltDirection::kSTOP;
}

DriverSeatDir_e Convert_SeatTiltDir_API2SOA(eevp::control::SoaSeatTiltDirection tiltDir)
{
	switch (tiltDir)
	{
	case eevp::control::SoaSeatTiltDirection::kSTOP:
		return eDriverSeatDir_Stop;
	case eevp::control::SoaSeatTiltDirection::kUP:
		return eDriverSeatDir_Up;
	case eevp::control::SoaSeatTiltDirection::kDOWN:
		return eDriverSeatDir_Down;
	default:
		break;
	}

	return eDriverSeatDir_Stop;
}