#ifndef IDRIVER_SEAT_LISTENER_H
#define IDRIVER_SEAT_LISTENER_H

#include "eevp/control/impl_type_soadeviceisnormal.h"

namespace eevp {
namespace control {
namespace driverseat {

class IDriverSeatListener {
public:
    virtual ~IDriverSeatListener() {};

    virtual void notifyIsDeviceNormal(const eevp::control::SoaDeviceIsNormal& fieldValue) = 0;
    virtual void getIsDeviceNormal() = 0;
};

} /// namespace driverseat
} /// namespace control
} /// namespace eevp

#endif /* IDRIVER_SEAT_LISTENER_H */