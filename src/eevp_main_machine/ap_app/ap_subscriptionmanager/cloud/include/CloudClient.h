#ifndef SUBSCRIPTION_CLOUD_CLIENT_H_
#define SUBSCRIPTION_CLOUD_CLIENT_H_

#include "ara/log/logger.h"

#include "CloudHelper.h"

namespace eevp {
namespace subscription {
namespace cloud {

class CloudClient {
public:
    CloudClient();
    ~CloudClient();

    void init();

    bool Login();
    void RefreshToken();
    ServerResponse PushInfo(eevp::ota::subscription::type::swClusterInfoList list);
    ServerResponse PullInfo(eevp::type::String& _accessToken,
                            eevp::type::String& _refreshToken,
                            eevp::ota::subscription::type::downloadPackageInfoList& list);
    ServerResponse SubInfo(std::vector<eevp::subscription::type::SubscriptionInfo>& list);

private:
    ara::log::Logger& mLogger;

    CloudHelper* pCloudHelper;
    RestApi loginRestHandler;
    RestApi refreshRestHandler;
    RestApi pushRestHandler;
    RestApi pullRestHandler;
    RestApi subRestHandler;

    std::string accessToken;
    std::string refreshToken;
    eevp::ota::subscription::type::swClusterInfoList swClusterInfos;
    std::vector<eevp::subscription::type::SubscriptionInfo> savelist;

    std::string LOGINURL;
    std::string PUSHURL;
    std::string PULLURL;
    std::string REFRESHURL;
    std::string SUBURL;
};
} /// namespace cloud
} /// namespace subscription
} /// namespace eevp

#endif /// SUBSCRIPTION_CLOUD_CLIENT_H_
