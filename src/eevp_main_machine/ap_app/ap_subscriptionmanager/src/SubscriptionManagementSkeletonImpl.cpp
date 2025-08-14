#include "SubscriptionManagementSkeletonImpl.h"

using namespace eevp::subscription;
using namespace eevp::subscription::service;
using namespace eevp::subscription::type;

namespace eevp {
namespace subscription {

SubscriptionManagementSkeletonImpl::SubscriptionManagementSkeletonImpl(
        ara::core::InstanceSpecifier instanceSpec,
        ara::com::MethodCallProcessingMode mode) :
    SubscriptionManagementSkeleton(instanceSpec, mode),
    mLogger(ara::log::CreateLogger("SUBM", "MGR", ara::log::LogLevel::kInfo)) {
}

SubscriptionManagementSkeletonImpl::~SubscriptionManagementSkeletonImpl() {
    StopOfferService();
}

void
SubscriptionManagementSkeletonImpl::setEventListener(std::shared_ptr<ISubscriptionManagementListener> _listener) {
    listener = _listener;
}

ara::core::Future<skeleton::SubscriptionManagementSkeleton::getSubscriptionInfoOutput>
SubscriptionManagementSkeletonImpl::getSubscriptionInfo(const eevp::type::String& appName) {
    skeleton::SubscriptionManagementSkeleton::getSubscriptionInfoOutput response;
    ara::core::Promise<skeleton::SubscriptionManagementSkeleton::getSubscriptionInfoOutput> promise;

    // TODO
    if (listener != nullptr) {
        listener->getSubscriptionInfo(appName);
    }

    response.isSubscription = false;
    mLogger.LogInfo() << "[getSubscriptionInfo] : " << response.isSubscription;

    promise.set_value(response);
    return promise.get_future();
}

void
SubscriptionManagementSkeletonImpl::sendNotifySubscriptionInfo(SubscriptionInfo& info) {
    notifySubscriptionInfo.Send(info);
}

} /// namespace subscription
} /// namespace eevp
