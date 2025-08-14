#ifndef EEVP_OTA_SUBSCRIPTION_PROXY_IMPL_H_
#define EEVP_OTA_SUBSCRIPTION_PROXY_IMPL_H_

#include <condition_variable>
#include <mutex>
#include <thread>

#include "eevp/ota/subscription/service/otasubscription_proxy.h"
#include "ara/log/logger.h"

#include "IOtaSubscriptionListener.h"

namespace eevp {
namespace subscription {

class OtaSubscriptionProxyImpl {
public:
    OtaSubscriptionProxyImpl();
    ~OtaSubscriptionProxyImpl();

    void setEventListener(std::shared_ptr<eevp::subscription::IOtaSubscriptionListener> _listener);
    bool init();

    bool requestAppInstall(const eevp::type::String& serviceName);
    bool requestAppUpdate(const eevp::type::String& serviceName);
    bool requestAppRollback();
    bool requestOtaProcess(const eevp::type::String& accessToken,
                            const eevp::type::String& refreshToken,
                            const eevp::ota::subscription::type::downloadPackageInfoList& downloadPackageInfoList);

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::ota::subscription::service::proxy::OtaSubscriptionProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeNoitfyAppEvent();
    void SubscribeNotifyRollbackComplete();
    void SubscribeNotifyUpdatableApp();
    void SubscribeNotifyUpdateComplete();
    void SubscribeNotifySwClusterInfo();
    void SubscribeNotifyResultInfo();

    /// @brief Unsubscribe Event
    void UnsubscribeNoitfyAppEvent();
    void UnsubscribeNotifyRollbackComplete();
    void UnsubscribeNotifyUpdatableApp();
    void UnsubscribeNotifyUpdateComplete();
    void UnsubcribeNotifySwClusterInfo();
    void UnsubscribeNotifyResultInfo();

    // callback func
    void cbAppEvent();
    void cbRollbackComplete();
    void cbUpdatableApp();
    void cbUpdateComplete();
    void cbSwClusterInfo();
    void cbResultInfo();

    ara::log::Logger& mLogger;
    std::shared_ptr<eevp::subscription::IOtaSubscriptionListener> listener;
    std::shared_ptr<eevp::ota::subscription::service::proxy::OtaSubscriptionProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;

    eevp::ota::subscription::type::RequestAppInfo requestAppInfo;
    eevp::ota::subscription::type::UpdatableAppInfo updatableAppInfo;
    bool retVal;

    std::mutex mHandle;
    std::condition_variable cvHandle;
};

} // namespace subscription
} // namespace eevp

#endif /// EEVP_OTA_SUBSCRIPTION_PROXY_IMPL_H_