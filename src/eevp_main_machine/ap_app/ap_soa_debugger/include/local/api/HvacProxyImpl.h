#ifndef __EEVP_CONTROL_HVAC_PROXY_IMPL_H__
#define __EEVP_CONTROL_HVAC_PROXY_IMPL_H__

#include <thread>

#include "eevp/control/soahvac_proxy.h"
#include <Common.h>
#include <SoaDataType.h>
#include "IHvacListener.h"

namespace eevp {
namespace control {
namespace hvac {

class HvacProxyImpl {
public:
    static HvacProxyImpl *GetInstance();
    static void DeleteInstance();

    HvacProxyImpl();
    ~HvacProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::hvac::IHvacListener> _listener);
    bool init();

    // method
    void SetHvacPower(const bool& on);
    void SetAcOnOff(const bool& on);
    void SetAutoTemp(const bool& on);
    void SetSync(const bool& on);
    void SetDriverTemp(const std::int8_t& temperature);
    void SetPassengerTemp(const std::int8_t& temperature);
    void SetBlowingForce(const std::uint8_t& force);
    void SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir);
    void SetAirSource(const eevp::control::SoaHvacAirSource& src);
    void SetForwardDefrostOn(const bool& on);
    void SetBackwardDefrostOn(const bool& on);

    // field getter
    bool getterIsDeviceNormal(DeviceNormal_e &isDeviceNormal);
    bool getterHvacSetting(HvacStatus_t &setting);
    bool getterHvacStatus(HvacStatus_t &status);
    bool getterHvacTemps(HvacTemps_t &temps);
    bool getterHvacSwVersion(int &version);

    std::string GetSubscriptStatusString();

private:
    static HvacProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaHvacProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbIsDeviceNormal();
    void cbHvacSetting();
    void cbHvacStatus();
    void cbHvacTemps();

    std::shared_ptr<eevp::control::hvac::IHvacListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaHvacProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace hvac
} // namespace control
} // namespace eevp

#endif /// __EEVP_CONTROL_HVAC_PROXY_IMPL_H__