#ifndef SUBSCRIPTION_CLOUD_HELPER_H
#define SUBSCRIPTION_CLOUD_HELPER_H

#include <fstream>
#include <map>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

#include <ara/log/logger.h>
#include <nlohmann/json.hpp>

#include "CloudDataType.h"
#include "rest_api.h"

namespace eevp {
namespace subscription {
namespace cloud {

class CloudHelper {
public:
    static CloudHelper* getInstance();

    ~CloudHelper();

    bool readConfig(std::string& loginurl, std::string& pushurl, std::string& pullurl, std::string& refreshurl, std::string& suburl);
    bool readSubInfo(std::vector<eevp::subscription::type::SubscriptionInfo>& list);
    bool getLoginInfo(std::string& vin, std::string& password);
    bool saveSubInfo(std::vector<eevp::subscription::type::SubscriptionInfo> list);
private:
    CloudHelper();
    CloudHelper(const CloudHelper&) = delete;
    CloudHelper& operator=(const CloudHelper&) = delete;

    static CloudHelper* instance;
    ara::log::Logger& mLogger;

    RestApi restHandle;

    std::string serverUrl;
    std::string vin;
    std::string password;
    std::string localtest;
};
} /// namespace cloud
} /// namespace subscription
} /// namespace eevp

#endif  // SUBSCRIPTION_CLOUD_HELPER_H
