#ifndef I_WIPER_LISTENER_H
#define I_WIPER_LISTENER_H

#include "eevp/control/impl_type_soadeviceisnormal.h"

namespace eevp {
namespace control {
namespace wiper {

class IWiperListener {
public:
    virtual ~IWiperListener() {};

    virtual void notifyIsDeviceNormal(const eevp::control::SoaDeviceIsNormal& fieldValue) = 0;
    virtual void getIsDeviceNormal() = 0;
};

} /// namespace roa
} /// namespace control
} /// namespace eevp

#endif /* I_WIPER_LISTENER_H */