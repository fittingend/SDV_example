#ifndef EEVP_CONTROL_POWER_PROXY_IMPL_H_
#define EEVP_CONTROL_POWER_PROXY_IMPL_H_

#include <thread>

#include "eevp/control/soapower_proxy.h"
#include "IPowerListener.h"

namespace eevp {
namespace control {
namespace power {

class PowerProxyImpl {
public:
    static PowerProxyImpl *GetInstance();
    static void DeleteInstance();

    PowerProxyImpl();
    ~PowerProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::power::IPowerListener> _listener);
    bool init();

    // field getter
    bool getterSoaPowerDeviceNormal(eevp::control::SoaDeviceIsNormal& isNormal);

    std::string GetSubscriptStatusString();

private:
    static PowerProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaPowerProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbFieldDeviceNormal();

    std::shared_ptr<eevp::control::power::IPowerListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaPowerProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace power
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_POWER_PROXY_IMPL_H_