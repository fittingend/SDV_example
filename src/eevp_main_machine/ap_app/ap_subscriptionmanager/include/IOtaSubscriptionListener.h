#ifndef IOTA_SUBSCRIPTION_LISTENER_H
#define IOTA_SUBSCRIPTION_LISTENER_H

namespace eevp {
namespace subscription {

class IOtaSubscriptionListener {
public:
    virtual ~IOtaSubscriptionListener() {};

    virtual void notifyAppEvent(const eevp::ota::subscription::type::RequestAppInfo& requestAppInfo) = 0;
    virtual void notifyRollbackComplete(const bool retVal) = 0;
    virtual void notifyUpdatableApp(const eevp::ota::subscription::type::UpdatableAppInfo& updatableAppInfo) = 0;
    virtual void notifyUpdateComplete(const bool retVal) = 0;
    virtual void notifySwClusterInfo(const eevp::ota::subscription::type::swClusterInfoList swClusterInfoList) = 0;
    virtual void notifyResultInfo(const eevp::ota::subscription::type::resultInfoList resultInfoList) = 0;
};

} /// namespace subscription
} /// namespace eevp

#endif /* IOTA_SUBSCRIPTION_LISTENER_H */