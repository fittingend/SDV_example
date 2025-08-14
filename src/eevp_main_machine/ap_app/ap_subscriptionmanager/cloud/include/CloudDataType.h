#ifndef SUBSCRIPTION_CLOUD_DATA_TYPE_H_
#define SUBSCRIPTION_CLOUD_DATA_TYPE_H_

#include "eevp/type/impl_type_string.h"
#include "eevp/ota/subscription/type/impl_type_downloadpackageinfolist.h"
#include "eevp/ota/subscription/type/impl_type_resultinfolist.h"
#include "eevp/ota/subscription/type/impl_type_swclusterinfolist.h"
#include "eevp/subscription/type/impl_type_subscriptioninfo.h"

namespace eevp {
namespace subscription {
namespace cloud {

    const char* const CONFIG_FILE                   = "/opt/para/SubscriptionManager/config.json";
    const char* const SUBINFO_FILE                  = "/opt/para/SubscriptionManager/subinfo.json";

    enum class ServerResponse {
        Ok,
        Error,
        TokenExpired
    };
} /// namespace cloud
} /// namespace subscription
} /// namespace eevp

#endif /// SUBSCRIPTION_CLOUD_DATA_TYPE_H_
