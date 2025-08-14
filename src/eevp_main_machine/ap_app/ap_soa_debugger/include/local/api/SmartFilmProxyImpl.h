#ifndef __EEVP_CONTROL_SMARTFILM_PROXY_IMPL_H__
#define __EEVP_CONTROL_SMARTFILM_PROXY_IMPL_H__

#include <thread>

#include "eevp/control/soasmartfilm_proxy.h"
#include "ISmartFilmListener.h"

namespace eevp {
namespace control {
namespace smartfilm {

class SmartFilmProxyImpl {
public:
    static SmartFilmProxyImpl *GetInstance();
    static void DeleteInstance();

    SmartFilmProxyImpl();
    ~SmartFilmProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::smartfilm::ISmartFilmListener> _listener);
    bool init();

    // method
    void SetOpacity(SmtfilmPos_e pos, int opacities);
    void SetAllOpacities(int opacities);
    void SetAuto(bool autoMode);

    // field getter
    bool getterIsDeviceNormal(DeviceNormal_e &isDeviceNormal);
    bool getterSmartFilmAutoMode(bool &autoMode);
    bool getterSmartFilmOpacities(SmtfilmOpacities_t &opacities);
    bool getterSmartFilmSwVersion(int &version);

    std::string GetSubscriptStatusString();

private:
    static SmartFilmProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaSmartFilmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbIsDeviceNormal();
    void cbSmartFilmAutoMode();
    void cbSmartFilmOpacities();

    std::shared_ptr<eevp::control::smartfilm::ISmartFilmListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaSmartFilmProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace smartfilm
} // namespace control
} // namespace eevp

#endif /// __EEVP_CONTROL_SMARTFILM_PROXY_IMPL_H__