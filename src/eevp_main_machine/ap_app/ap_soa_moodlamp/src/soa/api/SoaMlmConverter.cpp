#include <Common.h>
#include <SoaDataType.h>
#include <SoaMlmConverter.hpp>

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
eevp::control::SoaMlmMoodMode Convert_MoodMode_SOA2API(MlmMoodMode_e mode)
{
	switch (mode)
	{
	case eMlmMoodMode_CareMood:
		return eevp::control::SoaMlmMoodMode::kCARE_MOOD;
	case eMlmMoodMode_ComfortMood:
		return eevp::control::SoaMlmMoodMode::kCOMPORT_MOOD;
	case eMlmMoodMode_DelightMood:
		return eevp::control::SoaMlmMoodMode::kDELIGHT_MOOD;
	case eMlmMoodMode_MoodTherapy:
		return eevp::control::SoaMlmMoodMode::kMOOD_THERAPHY;
	case eMlmMoodMode_VitalityMood:
		return eevp::control::SoaMlmMoodMode::kVITALITY_MOOD;
	default:
		break;
	}

	return eevp::control::SoaMlmMoodMode::kCARE_MOOD;
}

MlmMoodMode_e Convert_MoodMode_API2SOA(eevp::control::SoaMlmMoodMode mode)
{
	switch (mode)
	{
	case eevp::control::SoaMlmMoodMode::kCARE_MOOD:
		return eMlmMoodMode_CareMood;
	case eevp::control::SoaMlmMoodMode::kCOMPORT_MOOD:
		return eMlmMoodMode_ComfortMood;
	case eevp::control::SoaMlmMoodMode::kDELIGHT_MOOD:
		return eMlmMoodMode_DelightMood;
	case eevp::control::SoaMlmMoodMode::kMOOD_THERAPHY:
		return eMlmMoodMode_MoodTherapy;
	case eevp::control::SoaMlmMoodMode::kVITALITY_MOOD:
		return eMlmMoodMode_VitalityMood;
	default:
		break;
	}

	return eMlmMoodMode_CareMood;
}


