#ifndef __IZONE2_ROA_LISTENER_HPP__
#define __IZONE2_ROA_LISTENER_HPP__

#include <thread>

#include <zone2/roa/control/impl_type_ecmroasnsrstatus.h>

namespace zone2
{
namespace roa
{
namespace control
{

class IZone2RoaListener {
public:
	virtual ~IZone2RoaListener() {};
	virtual void notifySnsrStatus(const zone2::roa::control::EcmRoaSnsrStatus &fieldValue) = 0;
	virtual void getSnsrStatus() = 0;
};

}
}
}

#endif // __IZONE2_ROA_LISTENER_HPP__