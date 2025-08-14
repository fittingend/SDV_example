#ifndef ISUBSCRIPTION_MANAGEMENT_LISTENER_H
#define ISUBSCRIPTION_MANAGEMENT_LISTENER_H

namespace eevp {
namespace subscription {

class ISubscriptionManagementListener {
public:
    virtual ~ISubscriptionManagementListener() {};
    virtual bool getSubscriptionInfo(const ara::core::String& appName) = 0;
};

} /// namespace subscription
} /// namespace eevp

#endif /* ISUBSCRIPTION_MANAGEMENT_LISTENER_H */