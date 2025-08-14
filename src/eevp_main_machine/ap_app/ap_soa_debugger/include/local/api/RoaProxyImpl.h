#ifndef EEVP_CONTROL_ROA_PROXY_IMPL_H_
#define EEVP_CONTROL_ROA_PROXY_IMPL_H_

#include <thread>

#include "eevp/control/soaroa_proxy.h"
#include "IRoaListener.h"

namespace eevp {
namespace control {
namespace roa {

class RoaProxyImpl {
public:
    static RoaProxyImpl *GetInstance();
    static void DeleteInstance();

    RoaProxyImpl();
    ~RoaProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::roa::IRoaListener> _listener);
    bool init();

    bool StartRoa();
    bool StopRoa();
    bool ResetRoaDetectInfo();

    // field getter
    bool getterIsDeviceNormal(eevp::control::SoaDeviceIsNormal& isDeviceNormal);

    std::string GetSubscriptStatusString();

private:
    static RoaProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaRoaProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbFieldDeviceNormal();
    void cbFieldSwVersion();

    void cbFieldDetectCount(); // soaRoaDetectCount
    void cbFieldDetectState(); // soaRoaDetectCount
    void cbFieldRoaMode(); // soaRoaMode
    void cbFieldRunningState(); // soaRoaRunningState
    void cbFieldSensorError(); // soaRoaSensorError

    std::shared_ptr<eevp::control::roa::IRoaListener> listener;
    std::shared_ptr<eevp::control::proxy::SoaRoaProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace roa
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_ROA_PROXY_IMPL_H_