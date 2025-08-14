#ifndef __IZONE5_DMS_LISTENER_HPP__
#define __IZONE5_DMS_LISTENER_HPP__

#include <thread>
#include <SoaDataType.h>

namespace zone5
{
namespace dms
{
namespace control
{

class IZone5DmsListener {
public:
	virtual ~IZone5DmsListener() {};
	virtual void notifyDeviceNormal(const DeviceNormal_e &isNormal) = 0;
	virtual void notifyDmsStatus(const bool &powerOn, const DmsDriverInside_e &inside) = 0;
	virtual void notifyDriverStatus(const DmsFatigue_e &fatigue, const DmsGazingDir_e &dir, const DmsOnPhone_e &phone, const DmsSmoking_e &smoking) = 0;
};

}
}
}

#endif // __IZONE5_DMS_LISTENER_HPP__