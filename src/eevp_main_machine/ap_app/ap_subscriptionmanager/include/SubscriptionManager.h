#include <csignal>
#include <thread>

#include <ara/log/logger.h>

#include "OtaSubscriptionProxyImpl.h"
#include "SubscriptionManagementSkeletonImpl.h"

#include "CloudClient.h"

namespace eevp {
namespace subscription {

class SubscriptionManager {
public:
    /// @brief Constructor
    SubscriptionManager();
    /// @brief Destructor
    ~SubscriptionManager();

    /// @brief Start S/W Component
    bool Start();
    /// @brief Run S/W Component
    void Run(int ac, char** av);
    /// @brief Terminate S/W Component
    void Terminate();

    // IOtaSubscriptionListener
    void notifyAppEvent(const eevp::ota::subscription::type::RequestAppInfo& requestAppInfo);
    void notifyRollbackComplete(const bool retVal);
    void notifyUpdatableApp(const eevp::ota::subscription::type::UpdatableAppInfo& updatableAppInfo);
    void notifyUpdateComplete(const bool retVal);
    void notifySwClusterInfo(const eevp::ota::subscription::type::swClusterInfoList swClusterInfoList);
    void notifyResultInfo(const eevp::ota::subscription::type::resultInfoList resultInfoList);

    bool requestAppInstall(const eevp::type::String& serviceName);
    bool requestAppUpdate(const eevp::type::String& serviceName);
    bool requestAppRollback();

    // ISubscriptionManagementListener
    bool getSubscriptionInfo(const eevp::type::String& appName);
    void notifySubscriptionInfo(std::vector<eevp::subscription::type::SubscriptionInfo> subscriptionList);

    // ICloudClient Listener
    bool requestOtaProcess(const eevp::type::String& accessToken,
                           const eevp::type::String& refreshToken,
                           const eevp::ota::subscription::type::downloadPackageInfoList& downloadPackageInfoList);

private:
    /// @brief Signal Handler
    static void SignalHandler(std::int32_t signal);

    /// @brief Find Proxy
    bool startOtaSubscriptionProxy();

    /// @brief Start Stub
    bool startSubscriptionManagementStub();
    bool startCloudClient();

    void LoginThread();
    void PullThread();
    void SubThread();
    void RefreshToken();

    /// @brief Flag of Running
    static std::atomic_bool mRunning;
    /// @brief Logger
    ara::log::Logger& mLogger;

    bool loginFlag;
    bool pullFlag;
    bool pushFlag;

    eevp::type::String accessToken;
    eevp::type::String refreshToken;
    eevp::ota::subscription::type::downloadPackageInfoList downloadlist;
    std::vector<eevp::subscription::type::SubscriptionInfo> sublist;

    std::shared_ptr<eevp::subscription::OtaSubscriptionProxyImpl> otaSubscriptionProxyImpl;
    std::shared_ptr<eevp::subscription::SubscriptionManagementSkeletonImpl> subscriptionManagementSkeletonImpl;
    std::shared_ptr<eevp::subscription::cloud::CloudClient> cloudClient;
};

}   // namespace subscription
}   // namespace eevp
