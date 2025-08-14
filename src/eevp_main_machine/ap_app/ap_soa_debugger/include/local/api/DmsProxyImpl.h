#ifndef __EEVP_CONTROL_DMS_PROXY_IMPL_H__
#define __EEVP_CONTROL_DMS_PROXY_IMPL_H__

#include <thread>

#include "eevp/control/soadms_proxy.h"
#include <Common.h>
#include <SoaDataType.h>
#include "IDmsListener.h"

namespace eevp {
namespace control {
namespace dms {

class DmsProxyImpl {
public:
    static DmsProxyImpl *GetInstance();
    static void DeleteInstance();

    DmsProxyImpl();
    ~DmsProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::dms::IDmsListener> _listener);
    bool init();

    // method
    void SetPower(bool on);

    // field getter
    bool getterIsDeviceNormal(DeviceNormal_e &isDeviceNormal);
    bool getterDmsStatus(bool &powerOn, DmsDriverInside_e &inside);
    bool getterDmsDriverStatus(DmsFatigue_e &fatigue, DmsGazingDir_e &dir, DmsOnPhone_e &phone, DmsSmoking_e &smoking);

    std::string GetSubscriptStatusString();

private:
    static DmsProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaDmsProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbOnDriverNotFocusing();
    void cbOnDriverNotWaken();
    void cbOnDriverNotWatchingFront();
    void cbIsDeviceNormal();
    void cbDmsStatus();
    void cbDriverStatus();

    std::shared_ptr<eevp::control::dms::IDmsListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaDmsProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace dms
} // namespace control
} // namespace eevp

#endif /// __EEVP_CONTROL_DMS_PROXY_IMPL_H__