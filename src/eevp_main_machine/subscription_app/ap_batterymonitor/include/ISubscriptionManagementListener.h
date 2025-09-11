#ifndef ISUBSCRIPTION_MANAGEMENT_LISTENER_H
#define ISUBSCRIPTION_MANAGEMENT_LISTENER_H
#include "eevp/subscription/type/impl_type_subscriptioninfo.h"

namespace eevp {
namespace subscription {
namespace service {

class ISubscriptionManagementListener {
public:
    virtual ~ISubscriptionManagementListener() {};
    virtual void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& subscriptionInfo) = 0;
   

};

} /// namespace service
} /// namespace subscription
} /// namespace eevp

#endif /* ISUBSCRIPTION_MANAGEMENT_LISTENER_H */