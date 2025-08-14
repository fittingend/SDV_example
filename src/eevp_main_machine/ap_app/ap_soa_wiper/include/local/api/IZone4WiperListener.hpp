#ifndef __IZONE4_WIPER_LISTENER_HPP__
#define __IZONE4_WIPER_LISTENER_HPP__

#include <thread>

#include <zone4/wiper/control/impl_type_ecuwiperoperationstatus.h>

namespace zone4
{
namespace wiper
{
namespace control
{

class IZone4WiperListener {
public:
	virtual ~IZone4WiperListener() {};
	virtual void notifyWiperOpStatus(const zone4::wiper::control::EcuWiperOperationStatus &fieldValue) = 0;
	virtual void getWiperOpStatus() = 0;
};

}
}
}

#endif // __IZONE4_WIPER_LISTENER_HPP__