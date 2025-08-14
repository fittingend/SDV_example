#ifndef I_ROA_LISTENER_H
#define I_ROA_LISTENER_H

#include "eevp/control/impl_type_soadeviceisnormal.h"

namespace eevp {
namespace control {
namespace roa {

class IRoaListener {
public:
    virtual ~IRoaListener() {};

    virtual void notifyIsDeviceNormal(const eevp::control::SoaDeviceIsNormal& fieldValue) = 0;
    virtual void getIsDeviceNormal() = 0;
};

} /// namespace roa
} /// namespace control
} /// namespace eevp

#endif /* I_ROA_LISTENER_H */