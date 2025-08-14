#ifndef __IZONE5_HVAC_LISTENER_HPP__
#define __IZONE5_HVAC_LISTENER_HPP__

#include <SoaDataType.h>

namespace zone5
{
namespace hvac
{
namespace control
{

class IZone5HvacListener {
public:
	virtual ~IZone5HvacListener() {};
	virtual void notifyDeviceNormal(const DeviceNormal_e &isNormal) = 0;
	virtual void notifyHvacSetting(const HvacStatus_t &setting) = 0;
	virtual void notifyHvacStatus(const HvacStatus_t &status) = 0;
	virtual void notifyHvacTemps(const HvacTemps_t &status) = 0;
};

}
}
}

#endif // __IZONE5_HVAC_LISTENER_HPP__