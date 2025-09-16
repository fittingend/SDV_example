#pragma once

#include "eevp/control/soadms_proxy.h"

namespace eevp {
namespace control {
namespace dms {

class IDmsListener {
public:
    virtual ~IDmsListener() = default;
    virtual void notifyDmsStatus(const eevp::control::SoaDmsStatus& status) = 0;
};

} // namespace dms
} // namespace control
} // namespace eevp
