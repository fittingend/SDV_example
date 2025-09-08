#ifndef IBMS_INFO_LISTENER_H
#define IBMS_INFO_LISTENER_H
// #include "eevp/bmsinfo/impl_type_ems_bmsinfo.h"

namespace eevp {
namespace bmsinfo {
namespace service {

class IBmsInfoListener {
public:
    virtual ~IBmsInfoListener() {};
    // virtual void notifyBmsInfo(const eevp::subscription::type::SubscriptionInfo& subscriptionInfo) = 0;
   

};

} /// namespace service
} /// namespace bmsinfo
} /// namespace eevp

#endif /* IBMS_INFO_LISTENER_H */