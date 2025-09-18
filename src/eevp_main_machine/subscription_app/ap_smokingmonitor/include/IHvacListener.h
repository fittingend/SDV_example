#pragma once

#include "eevp/control/soahvac_proxy.h"

namespace eevp {
namespace control {
namespace hvac {

class IHvacListener {
public:
    virtual ~IHvacListener() = default;
    virtual void notifyHvacStatus(const eevp::control::SoaHvacStatus& status) = 0;
};

} // namespace hvac
} // namespace control
} // namespace eevp