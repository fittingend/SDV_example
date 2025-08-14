#ifndef EEVP_CONTROL_DRIVERSEAT_PROXY_IMPL_H_
#define EEVP_CONTROL_DRIVERSEAT_PROXY_IMPL_H_

#include <thread>

#include "eevp/control/soadriverseat_proxy.h"
#include "IDriverSeatListener.h"

namespace eevp {
namespace control {
namespace driverseat {

class DriverSeatProxyImpl {
public:
    static DriverSeatProxyImpl *GetInstance();
    static void DeleteInstance();

    DriverSeatProxyImpl();
    ~DriverSeatProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::driverseat::IDriverSeatListener> _listener);
    bool init();

    // field getter
    bool getterIsDeviceNormal(eevp::control::SoaDeviceIsNormal& isDeviceNormal);

    void RequestMoveDrvSeatHeight(SoaSeatHeightDirection dir);
    void RequestMoveDrvSeatRecline(SoaSeatReclineDirection dir);
    void RequestMoveDrvSeatSlide(SoaSeatSlideDirection dir);
    void RequestMoveDrvSeatTilt(SoaSeatTiltDirection dir);

    std::string GetSubscriptStatusString();

private:
    static DriverSeatProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaDriverSeatProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbIsDeviceNormal();
    void cbDrvSeatMotorDirection();
    void cbDrvSeatMotorPosition();

    std::shared_ptr<eevp::control::driverseat::IDriverSeatListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaDriverSeatProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace driverseat
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_DRIVERSEAT_PROXY_IMPL_H_