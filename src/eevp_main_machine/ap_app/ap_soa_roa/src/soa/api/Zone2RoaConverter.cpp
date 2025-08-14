#include <Common.h>
#include <SoaDataType.h>
#include <Zone2RoaConverter.hpp>

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

// Sensor Error
std::uint8_t Convert_SensorError_SOA2Zone(RoaSensorError_e err)
{
	switch (err)
	{
	case eRoaSensorError_Ok:
		return 0;
	case eRoaSensorError_Snsr:
		return 1;
	case eRoaSensorError_Blckge:
		return 2;
	default:
		break;
	}

	return 1;
}

RoaSensorError_e Convert_SensorError_Zone2SOA(std::uint8_t err)
{
	switch (err)
	{
	case 0:
		return eRoaSensorError_Ok;
	case 1:
		return eRoaSensorError_Snsr;
	case 2:
		return eRoaSensorError_Blckge;
	default:
		break;
	}

	return eRoaSensorError_Snsr;
}

// DetectInfo2Count
int Convert_DetectInfo2Count_Zone2SOA(std::uint8_t detInfo)
{
	return (detInfo == 1) ? 1 : 0;
}


