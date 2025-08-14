#include <Common.h>
#include <SoaDataType.h>
#include <SoaRoaConverter.hpp>

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

// RunningState
eevp::control::SoaRoaRunningState Convert_RunningState_SOA2API(RoaRunningState_e runningState)
{
	switch (runningState)
	{
	case eRoaRunningState_Off:
		return eevp::control::SoaRoaRunningState::kOFF;
	case eRoaRunningState_On:
		return eevp::control::SoaRoaRunningState::kON;
	default:
		break;
	}

	return eevp::control::SoaRoaRunningState::kOFF;
}

RoaRunningState_e Convert_RunningState_API2SOA(eevp::control::SoaRoaRunningState runningState)
{
	switch (runningState)
	{
	case eevp::control::SoaRoaRunningState::kOFF:
		return eRoaRunningState_Off;
	case eevp::control::SoaRoaRunningState::kON:
		return eRoaRunningState_On;
	default:
		break;
	}

	return eRoaRunningState_Off;
}

// ROA Mode
eevp::control::SoaRoaMode Convert_RoaMode_SOA2API(RoaMode_e mode)
{
	switch (mode)
	{
	case eRoaMode_Off:
		return eevp::control::SoaRoaMode::kOFF;
	case eRoaMode_Normal:
		return eevp::control::SoaRoaMode::kNORMAL;
	case eRoaMode_Advanced:
		return eevp::control::SoaRoaMode::kADVANCED;
	default:
		break;
	}

	return eevp::control::SoaRoaMode::kNORMAL;
}

RoaMode_e Convert_RoaMode_API2SOA(eevp::control::SoaRoaMode mode)
{
	switch (mode)
	{
	case eevp::control::SoaRoaMode::kOFF:
		return eRoaMode_Off;
	case eevp::control::SoaRoaMode::kNORMAL:
		return eRoaMode_Normal;
	case eevp::control::SoaRoaMode::kADVANCED:
		return eRoaMode_Advanced;
	default:
		break;
	}

	return eRoaMode_Normal;
}

// Sensor Error
eevp::control::SoaRoaSensorError Convert_SensorError_SOA2API(RoaSensorError_e err)
{
	switch (err)
	{
	case eRoaSensorError_Ok:
		return eevp::control::SoaRoaSensorError::kOK;
	case eRoaSensorError_Snsr:
		return eevp::control::SoaRoaSensorError::kERR_SNSR;
	case eRoaSensorError_Blckge:
		return eevp::control::SoaRoaSensorError::kERR_BLCKGE;
	default:
		break;
	}

	return eevp::control::SoaRoaSensorError::kERR_SNSR;
}

RoaSensorError_e Convert_SensorError_API2SOA(eevp::control::SoaRoaSensorError err)
{
	switch (err)
	{
	case eevp::control::SoaRoaSensorError::kOK:
		return eRoaSensorError_Ok;
	case eevp::control::SoaRoaSensorError::kERR_SNSR:
		return eRoaSensorError_Snsr;
	case eevp::control::SoaRoaSensorError::kERR_BLCKGE:
		return eRoaSensorError_Blckge;
	default:
		break;
	}

	return eRoaSensorError_Snsr;
}

// Detect Count
eevp::control::SoaRoaDetectState Convert_DetectState_SOA2API(int count)
{
	if (count == 0)
	{
		return eevp::control::SoaRoaDetectState::kEMPTY;
	}
	else if (count == 1)
	{
		return eevp::control::SoaRoaDetectState::kDETECTED_ONCE;
	}
	else if (count > 1)
	{
		return eevp::control::SoaRoaDetectState::kDETECTED_SEVERAL;
	}

	return eevp::control::SoaRoaDetectState::kUNKNOWN;
}

int Convert_DetectState_API2SOA(eevp::control::SoaRoaDetectState count)
{
	switch (count)
	{
	case eevp::control::SoaRoaDetectState::kEMPTY:
		return 0;
	case eevp::control::SoaRoaDetectState::kDETECTED_ONCE:
		return 1;
	case eevp::control::SoaRoaDetectState::kDETECTED_SEVERAL:
		return 2;
	default:
		break;
	}

	return 0;
}

