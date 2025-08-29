#ifndef EEVP_SUBSCRIPTION_MANAGEMENT_PROXY_IMPL_H_
#define EEVP_SUBSCRIPTION_MANAGEMENT_PROXY_IMPL_H_

#include <ara/log/logger.h>
#include "eevp/subscription/service/subscriptionmanagement_proxy.h"
#include "eevp/subscription/type/impl_type_subscriptioninfo.h"
#include "ISubscriptionManagementListener.h"

namespace eevp {
namespace subscription {
namespace service {
    
class SubscriptionManagementProxyImpl {
public:
    SubscriptionManagementProxyImpl();
    ~SubscriptionManagementProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::subscription::service::ISubscriptionManagementListener> _listener);
    bool init();
    // method
    bool getSubscriptionInfo(eevp::type::String appName, bool& isSubscription);
    


private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::subscription::service::proxy::SubscriptionManagementProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeSubscriptionInfo();

    /// @brief Unsubscribe Event
    void UnsubscribeSubscriptionInfo();

    /// @brief callback func
    void cbSubscriptionInfo();

    ara::log::Logger& mLogger;
    std::shared_ptr<eevp::subscription::service::ISubscriptionManagementListener> listener;
    std::shared_ptr<eevp::subscription::service::proxy::SubscriptionManagementProxy> mProxy;

    std::unique_ptr<eevp::subscription::service::proxy::SubscriptionManagementProxy> mRPort{nullptr};
    /// @brief FindServiceHandle
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    /// @brief Current ProxyHandle
    eevp::subscription::service::proxy::SubscriptionManagementProxy::HandleType mProxyHandle;
    std::mutex mHandle;
    std::condition_variable cvHandle;
};

} // namespace service
} // namespace subscription
} // namespace eevp

#endif /// EEVP_SUBSCRIPTION_MANAGEMENT_PROXY_IMPL_H_