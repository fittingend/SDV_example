#include <Common.h>
#include <SoaDataType.h>
#include <SoaWiperConverter.hpp>

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

// Wiper Mode
eevp::control::SoaWiperMode Convert_WiperMode_SOA2API(WiperOpMode_e mode)
{
	switch (mode)
	{
	case eWiperOpMode_Off:
		return eevp::control::SoaWiperMode::kOFF;
	case eWiperOpMode_Mist:
		return eevp::control::SoaWiperMode::kON;
	case eWiperOpMode_Low:
		return eevp::control::SoaWiperMode::kON;
	case eWiperOpMode_High:
		return eevp::control::SoaWiperMode::kON;
	case eWiperOpMode_Int1:
		return eevp::control::SoaWiperMode::kINT1;
	case eWiperOpMode_Int2:
		return eevp::control::SoaWiperMode::kINT2;
	case eWiperOpMode_Int3:
		return eevp::control::SoaWiperMode::kINT3;
	case eWiperOpMode_Int4:
		return eevp::control::SoaWiperMode::kINT3;
	case eWiperOpMode_Int5:
		return eevp::control::SoaWiperMode::kINT3;
	default:
		break;
	}

	return eevp::control::SoaWiperMode::kOFF;
}

WiperOpMode_e Convert_WiperMode_API2SOA(eevp::control::SoaWiperMode mode)
{
	switch (mode)
	{
	case eevp::control::SoaWiperMode::kOFF:
		return eWiperOpMode_Off;
	case eevp::control::SoaWiperMode::kON:
		return eWiperOpMode_Low;
	case eevp::control::SoaWiperMode::kINT1:
		return eWiperOpMode_Int1;
	case eevp::control::SoaWiperMode::kINT2:
		return eWiperOpMode_Int2;
	case eevp::control::SoaWiperMode::kINT3:
		return eWiperOpMode_Int3;
	default:
		break;
	}

	return eWiperOpMode_Off;
}


