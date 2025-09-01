#pragma once

#include "eevp/control/soasmartfilm_proxy.h"

namespace eevp {
namespace control {
namespace smartfilm {

class ISmartFilmListener {
public:
    virtual ~ISmartFilmListener() = default;
    virtual void notifyFilmDeviceNormal(const eevp::control::SoaDeviceIsNormal& status) = 0;
};

} // namespace smartfilm
} // namespace control
} // namespace eevp
