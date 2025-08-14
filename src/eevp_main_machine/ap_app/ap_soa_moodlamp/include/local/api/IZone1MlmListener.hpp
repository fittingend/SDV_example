#ifndef __IZONE1_MLM_LISTENER_HPP__
#define __IZONE1_MLM_LISTENER_HPP__

#include <zone1/mlm/control/impl_type_ecumlmstate.h>

namespace zone1
{
namespace mlm
{
namespace control
{

class IZone1MlmListener {
public:
	virtual ~IZone1MlmListener() {};
	virtual void notifyMlmStatus(const zone1::mlm::control::EcuMlmState &fieldValue) = 0;
	virtual void getMlmStatus() = 0;
};

}
}
}

#endif // __IZONE1_MLM_LISTENER_HPP__