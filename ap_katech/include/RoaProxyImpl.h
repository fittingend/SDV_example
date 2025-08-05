#ifndef EEVP_ROA_PROXY_IMPL_H_
#define EEVP_ROA_PROXY_IMPL_H_

#include <ara/log/logger.h>
#include "eevp/control/soaroa_proxy.h"
#include "IRoaListener.h"

namespace eevp {
namespace control {
namespace roa {

class RoaProxyImpl {
public:
    RoaProxyImpl();
    ~RoaProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::roa::IRoaListener> _listener);
    bool init();

    // field getter
    bool getSoaRoaDetectState(eevp::control::SoaRoaDetectState& soaRoaDetectState);
    bool getSoaRoaDetectCount(std::uint8_t& soaRoaDetectCount);

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaRoaProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Field
    void SubscribeSoaRoaDetectCount();

    /// @brief Unsubscribe Field
    void UnsubscribeField();

    /// @brief callback func
    void cbSoaRoaDetectCount();
    //void StateChangeCallback(ara::core::String callsign, ara::com::SubscriptionState state);

    ara::log::Logger& mLogger;
    std::shared_ptr<eevp::control::roa::IRoaListener> listener;
    std::shared_ptr<eevp::control::proxy::SoaRoaProxy> mProxy;

    std::unique_ptr<eevp::control::proxy::SoaRoaProxy> mRPort{nullptr};
    /// @brief FindServiceHandle
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    /// @brief Current ProxyHandle
    eevp::control::proxy::SoaRoaProxy::HandleType mProxyHandle;
    std::mutex mHandle;
    std::condition_variable cvHandle;
};

} // namespace ROA
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_ROA_PROXY_IMPL_H_