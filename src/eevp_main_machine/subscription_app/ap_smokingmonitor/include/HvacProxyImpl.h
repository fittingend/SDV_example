#pragma once

#include <ara/log/logger.h>
#include <mutex>
#include <condition_variable>
#include "eevp/control/soahvac_proxy.h"
#include "IHvacListener.h"
#include "eevp/control/impl_type_soahvacblowdir.h"
#include "eevp/control/impl_type_soahvacairsource.h"
#include "eevp/control/impl_type_soahvacstatus.h"

namespace eevp
{
namespace control
{
namespace hvac
{

class HvacProxyImpl {
public :
    HvacProxyImpl();
    ~HvacProxyImpl();

    bool init();
    void SetHvacPower(const bool& on);                                  // Method 추가
    void SetBlowingForce(const std::uint8_t& force);                    // Method 추가
    void SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir); // Method 추가
    void SetAirSource(const eevp::control::SoaHvacAirSource& src);      // Method 추가
    bool getSoaHvacSetting(eevp::control::SoaHvacStatus& status);       // Getter

private :
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaHvacProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    void SubscribeSoaHvacSetting();
    void UnsubscribeField();
    void cbSoaHvacSetting();

    ara::log::Logger& mLogger;
    std::shared_ptr<IHvacListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaHvacProxy> mProxy;

    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    std::mutex mHandleMutex;
    std::condition_variable mCv;
};

}
}
}