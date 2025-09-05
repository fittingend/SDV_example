#pragma once

#include "eevp/control/soamlm_proxy.h"

namespace eevp {
namespace control {
namespace mlm {

class IMlmListener {
public:
    virtual ~IMlmListener() = default;
    virtual void notifyMlmStatus(const eevp::control::SoaMlmStatus& status) = 0;
};

} // namespace mlm
} // namespace control
} // namespace eevp
