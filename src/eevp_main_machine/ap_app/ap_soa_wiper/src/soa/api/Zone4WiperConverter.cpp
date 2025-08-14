#include <Common.h>
#include <SoaDataType.h>
#include <Zone4WiperConverter.hpp>

// DeviceNormal
std::uint8_t Convert_DeviceNormal_SOA2Zone(DeviceNormal_e isNormal)
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

DeviceNormal_e Convert_DeviceNormal_Zone2SOA(std::uint8_t isNormal)
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
std::uint8_t Convert_WiperMode_SOA2Zone(WiperOpMode_e wiperOp)
{
	switch (wiperOp)
	{
	case eWiperOpMode_Off:
		return 0;
	case eWiperOpMode_Mist:
		return 4;
	case eWiperOpMode_Low:
		return 4;
	case eWiperOpMode_High:
		return 4;
	case eWiperOpMode_Int1:
		return 1;
	case eWiperOpMode_Int2:
		return 2;
	case eWiperOpMode_Int3:
		return 3;
	case eWiperOpMode_Int4:
		return 3;
	case eWiperOpMode_Int5:
		return 3;
	default:
		break;
	}

	return 0;
}

WiperOpMode_e Convert_WiperMode_Zone2SOA(std::uint8_t wiperOp)
{
	switch (wiperOp)
	{
	case 0:
		return eWiperOpMode_Off;
	case 4:
		return eWiperOpMode_Low;
	case 1:
		return eWiperOpMode_Int1;
	case 2:
		return eWiperOpMode_Int2;
	case 3:
		return eWiperOpMode_Int3;
	default:
		break;
	}

	return eWiperOpMode_Off;

}

