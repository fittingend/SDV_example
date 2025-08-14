#ifndef __IZONE3_REARCURTAIN_LISTENER_IMPL_H__
#define __IZONE3_REARCURTAIN_LISTENER_IMPL_H__

#include "zone3/rcurtain/control/impl_type_ecurcurtainstatus.h"
#include "zone3/rcurtain/control/zone3rcurtain_proxy.h"

namespace zone3 {
namespace rcurtain {
namespace control {

class IZone3RearCurtainListener {
public:
	virtual ~IZone3RearCurtainListener() {};
	virtual void notifyRcurtainStatus(const zone3::rcurtain::control::EcuRcurtainStatus &fieldValue) = 0;
	virtual void getRcurtainStatus() = 0;
};

}
}
}

#endif