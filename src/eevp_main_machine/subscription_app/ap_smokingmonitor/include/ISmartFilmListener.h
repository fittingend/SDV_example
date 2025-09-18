#pragma once

#include "eevp/control/soasmartfilm_proxy.h"

namespace eevp {
namespace control {
namespace smartfilm {

class ISmartfilmListener {
public:
    virtual ~ISmartfilmListener() = default;
    virtual void notifySmartfilmStatus(const eevp::control::SoaFilmOpacityArray& status) = 0;
};

} // namespace smartfilm
} // namespace control
} // namespace eevp