#ifndef EEVP_CONTROL_REARCURTAIN_PROXY_IMPL_H_
#define EEVP_CONTROL_REARCURTAIN_PROXY_IMPL_H_

#include <ara/log/logger.h>
#include "eevp/control/soarcurtain_proxy.h"

namespace eevp {
namespace control {
namespace rearcurtain {

class RearCurtainProxyImpl {
public:
    RearCurtainProxyImpl();
    ~RearCurtainProxyImpl();

    //void setEventListener(const std::shared_ptr<eevp::control::rearcurtain::IRearCurtainListener> _listener);
    bool init();

    // method
    eevp::control::SoaErrorState requestRearCurtainOperation(const eevp::control::SoaRctnMotorDir& motorDir);
    void requestRearCurtainPosition(const std::uint8_t& posPercentage);

    // field getter
    bool getSoaRctnStatus(eevp::control::SoaRctnStatus& soaRctnStatus);

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaRcurtainProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    ara::log::Logger& mLogger;
    //std::shared_ptr<eevp::control::rearcurtain::IRearCurtainListener> listener;
    std::shared_ptr<eevp::control::proxy::SoaRcurtainProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    std::mutex mHandle;
    std::condition_variable cvHandle;
};

} // namespace rearcurtain
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_REARCURTAIN_PROXY_IMPL_H_