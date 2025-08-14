#ifndef __I_POWER_LISTENER__H__
#define __I_POWER_LISTENER__H__

#include "eevp/control/impl_type_soadeviceisnormal.h"

namespace eevp {
namespace control {
namespace power {

class IPowerListener {
public:
    virtual ~IPowerListener() {};
    virtual void notifyDeviceNormal(const eevp::control::SoaDeviceIsNormal & fieldValue) = 0;

    virtual void getSoaMlmStatus() = 0;
};

} /// namespace moodlamp
} /// namespace control
} /// namespace eevp

#endif /* __I_POWER_LISTENER__H__ */