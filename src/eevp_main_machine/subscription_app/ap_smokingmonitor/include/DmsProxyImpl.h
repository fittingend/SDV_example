#pragma once

#include <ara/log/logger.h>
#include <mutex>
#include <condition_variable>
#include "eevp/control/soadms_proxy.h"
#include "IDmsListener.h"
#include "eevp/control/impl_type_soadmsdriverstatus.h"

namespace eevp {
namespace control {
namespace dms {

class DmsProxyImpl {
public:
    DmsProxyImpl();
    ~DmsProxyImpl();

    void setEventListener(std::shared_ptr<IDmsListener> listener);
    bool init();
    void SetDmsPower(const bool& on);                                      // Method 추가
    // void getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status);
    bool getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status); // Getter 추가

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaDmsProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    void SubscribeSoaDmsDriverStatus();
    void UnsubscribeField();
    void cbSoaDmsDriverStatus();

    ara::log::Logger& mLogger;
    std::shared_ptr<IDmsListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaDmsProxy> mProxy;

    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    std::mutex mHandleMutex;
    std::condition_variable mCv;
};

} // namespace dms
} // namespace control
} // namespace eevp