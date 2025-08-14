#ifndef __IZONE1_DRVSEAT_LISTENER_HPP__
#define __IZONE1_DRVSEAT_LISTENER_HPP__

#include <thread>

#include <zone1/drvseat/control/impl_type_ecudrvseatmotordirection.h>
#include <zone1/drvseat/control/impl_type_ecudrvseatmotorposition.h>

namespace zone1
{
namespace drvseat
{
namespace control
{

class IZone1DrvseatListener {
public:
	virtual ~IZone1DrvseatListener() {};
	virtual void notifyMotorDir(const zone1::drvseat::control::EcuDrvSeatMotorDirection &fieldValue) = 0;
	virtual void getMotorDir() = 0;
	virtual void notifyMotorPos(const zone1::drvseat::control::EcuDrvseatMotorPosition &fieldValue) = 0;
	virtual void getMotorPos() = 0;
};

}
}
}

#endif // __IZONE1_DRVSEAT_LISTENER_HPP__