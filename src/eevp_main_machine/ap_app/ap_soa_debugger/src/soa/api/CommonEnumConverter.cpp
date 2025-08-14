#include <Common.h>
#include <SoaDataType.h>
#include <CommonEnumConverter.h>

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
