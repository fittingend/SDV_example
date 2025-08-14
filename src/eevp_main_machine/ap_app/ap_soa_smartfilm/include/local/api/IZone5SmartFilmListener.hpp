#ifndef __IZONE5_SMARTFILM_LISTENER_HPP__
#define __IZONE5_SMARTFILM_LISTENER_HPP__

#include <SoaDataType.h>

namespace zone5
{
namespace smtfilm
{
namespace control
{

class IZone5SmartFilmListener {
public:
	virtual ~IZone5SmartFilmListener() {};
	virtual void notifyDeviceNormal(const DeviceNormal_e &isNormal) = 0;
	virtual void notifyOpacities(const SmtfilmOpacities_t &opacities) = 0;

};

}
}
}

#endif // __IZONE5_SMARTFILM_LISTENER_HPP__