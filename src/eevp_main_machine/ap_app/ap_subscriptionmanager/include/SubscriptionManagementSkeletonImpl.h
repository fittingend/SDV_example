#ifndef EEVP_SUBSCRIPTION_MANAGEMENT_SKELETON_IMPL_H_
#define EEVP_SUBSCRIPTION_MANAGEMENT_SKELETON_IMPL_H_

#include "eevp/subscription/service/subscriptionmanagement_skeleton.h"
#include "ara/log/logger.h"

#include "ISubscriptionManagementListener.h"

namespace eevp {
namespace subscription {

class SubscriptionManagementSkeletonImpl : public eevp::subscription::service::skeleton::SubscriptionManagementSkeleton {
public:
    SubscriptionManagementSkeletonImpl(
            ara::core::InstanceSpecifier instanceSpec,
            ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEventSingleThread);

    ~SubscriptionManagementSkeletonImpl();

    void setEventListener(std::shared_ptr<eevp::subscription::ISubscriptionManagementListener> _listener);

    ara::core::Future<getSubscriptionInfoOutput> getSubscriptionInfo(const eevp::type::String& appName) override;

    void sendNotifySubscriptionInfo(eevp::subscription::type::SubscriptionInfo& info);

private:
    ara::log::Logger& mLogger;
    std::shared_ptr<eevp::subscription::ISubscriptionManagementListener> listener;
};

} /// namespace subscription
} /// namespace eevp

#endif /// EEVP_SUBSCRIPTION_MANAGEMENT_SKELETON_IMPL_H_
