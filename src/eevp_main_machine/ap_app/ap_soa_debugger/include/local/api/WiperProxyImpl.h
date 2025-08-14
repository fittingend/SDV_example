#ifndef EEVP_CONTROL_WIPER_PROXY_IMPL_H_
#define EEVP_CONTROL_WIPER_PROXY_IMPL_H_

#include <thread>

#include "eevp/control/soawiper_proxy.h"
#include "IWiperListener.h"

namespace eevp {
namespace control {
namespace wiper {

class WiperProxyImpl {
public:
    static WiperProxyImpl *GetInstance();
    static void DeleteInstance();

    WiperProxyImpl();
    ~WiperProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::wiper::IWiperListener> _listener);
    bool init();

    void RequestWiperOperation(eevp::control::SoaWiperMode wiperMode);

    // field getter
    bool getterIsDeviceNormal(eevp::control::SoaDeviceIsNormal& isDeviceNormal);

    std::string GetSubscriptStatusString();

private:
    static WiperProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaWiperProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbIsDeviceNormal();

    std::shared_ptr<eevp::control::wiper::IWiperListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaWiperProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace wiper
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_WIPER_PROXY_IMPL_H_