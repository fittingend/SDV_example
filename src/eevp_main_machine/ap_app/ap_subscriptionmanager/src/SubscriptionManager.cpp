#include "SubscriptionManager.h"

namespace eevp {
namespace subscription {

using eevp::subscription::cloud::ServerResponse;
std::atomic_bool SubscriptionManager::mRunning(false);

class OtaSubscriptionListener : public IOtaSubscriptionListener {
public:
    OtaSubscriptionListener(SubscriptionManager* mgr) : subscriptionManager(mgr) {}

    void notifyAppEvent(const eevp::ota::subscription::type::RequestAppInfo& requestAppInfo) {
        return subscriptionManager->notifyAppEvent(requestAppInfo);
    }

    void notifyRollbackComplete(const bool retVal) {
        return subscriptionManager->notifyRollbackComplete(retVal);
    }

    void notifyUpdatableApp(const eevp::ota::subscription::type::UpdatableAppInfo& updatableAppInfo) {
        return subscriptionManager->notifyUpdatableApp(updatableAppInfo);
    }

    void notifyUpdateComplete(const bool retVal) {
        return subscriptionManager->notifyUpdateComplete(retVal);
    }

    void notifySwClusterInfo(const eevp::ota::subscription::type::swClusterInfoList swClusterInfoList) {
        return subscriptionManager->notifySwClusterInfo(swClusterInfoList);
    }

    void notifyResultInfo(const eevp::ota::subscription::type::resultInfoList resultInfoList) {
        return subscriptionManager->notifyResultInfo(resultInfoList);
    }

private:
    SubscriptionManager* subscriptionManager;
};

class SubscriptionManagementListener : public ISubscriptionManagementListener {
public:
    SubscriptionManagementListener(SubscriptionManager* mgr) : subscriptionManager(mgr) {}

    bool getSubscriptionInfo(const ara::core::String& appName) {
        return subscriptionManager->getSubscriptionInfo(appName);
    }

private:
    SubscriptionManager* subscriptionManager;
};

SubscriptionManager::SubscriptionManager() :
        mLogger(ara::log::CreateLogger("SUBM", "SUBM", ara::log::LogLevel::kInfo)),
        cloudClient{nullptr},
        otaSubscriptionProxyImpl{nullptr},
        subscriptionManagementSkeletonImpl{nullptr},
        loginFlag(false), pullFlag(false), pushFlag(false) {
    // std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
}

SubscriptionManager::~SubscriptionManager() {
}

void
SubscriptionManager::SignalHandler(std::int32_t /*signal*/) {
    mRunning = false;
}

bool
SubscriptionManager::Start() {
    mLogger.LogInfo() << "SubscriptionManager Start";

    mRunning = true;

    if (!startOtaSubscriptionProxy()) {
        return false;
    }

    if (!startSubscriptionManagementStub()) {
        return false;
    }

    if (!startCloudClient()) {
        return false;
    }

    return true;
}

void
SubscriptionManager::Run(int ac, char** av) {
    mLogger.LogInfo() << "SubscriptionManager Run";

    LoginThread();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    PullThread();
    SubThread();

    while (mRunning) {
        mLogger.LogInfo() << "SubscriptionManager is alive";
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void
SubscriptionManager::Terminate() {
    mLogger.LogInfo() << "SubscriptionManager Terminate";
    mRunning = false;
}

void
SubscriptionManager::notifyAppEvent(const eevp::ota::subscription::type::RequestAppInfo& requestAppInfo) {
    // TODO
}

void
SubscriptionManager::notifyRollbackComplete(const bool retVal) {
}

void
SubscriptionManager::notifyUpdatableApp(const eevp::ota::subscription::type::UpdatableAppInfo& updatableAppInfo) {
}

void
SubscriptionManager::notifyUpdateComplete(const bool retVal) {
}

void
SubscriptionManager::notifySwClusterInfo(const eevp::ota::subscription::type::swClusterInfoList swClusterInfoList) {
    mLogger.LogInfo() << __func__;

    while (true) {
        ServerResponse sr = cloudClient->PushInfo(swClusterInfoList);

        if (sr == ServerResponse::TokenExpired) {
            mLogger.LogInfo() << "[Push] Token Expired!";
            RefreshToken();
        } else if (sr == ServerResponse::Error) {
            mLogger.LogInfo() << "[Push] Failed!";
        } else {
            mLogger.LogInfo() << "[Push] Success!";
            pushFlag = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

void
SubscriptionManager::notifyResultInfo(const eevp::ota::subscription::type::resultInfoList resultInfoList) {
    mLogger.LogInfo() << __func__;
}

bool
SubscriptionManager::requestAppInstall(const eevp::type::String& serviceName) {
    otaSubscriptionProxyImpl->requestAppInstall(serviceName);
    return true;
}

bool
SubscriptionManager::requestAppUpdate(const eevp::type::String& serviceName) {
    otaSubscriptionProxyImpl->requestAppUpdate(serviceName);
    return true;
}

bool
SubscriptionManager::requestAppRollback() {
    otaSubscriptionProxyImpl->requestAppRollback();
    return true;
}

void
SubscriptionManager::LoginThread() {
    std::thread login_thread([this]() {
        while (!cloudClient->Login()) {
            mLogger.LogInfo() << "[Login] Retry...";
            pullFlag = false;
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
        mLogger.LogInfo() << "[Login] Success!";
        pullFlag = true;
    });
    login_thread.detach();
}

void
SubscriptionManager::PullThread() {
    std::thread pull_thread([this]() {
        while (true) {
            if (pullFlag && pushFlag) {
                ServerResponse sr = cloudClient->PullInfo(accessToken, refreshToken, downloadlist);
                if (sr == ServerResponse::TokenExpired) {
                    mLogger.LogInfo() << "[Pull] Token Expired!";
                    RefreshToken();
                } else if (sr == ServerResponse::Error) {
                    mLogger.LogInfo() << "[Pull] Failed!";
                } else {
                    mLogger.LogInfo() << "[Pull] Success!";
                    requestOtaProcess(accessToken, refreshToken, downloadlist);
                }
            } else {
                mLogger.LogInfo() << "[Pull] Sleep...";
                std::this_thread::sleep_for(std::chrono::seconds(20));
            }
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    });
    pull_thread.detach();
}

void
SubscriptionManager::SubThread() {
    std::thread sub_thread([this]() {
        while (true) {
            sublist.clear();
            ServerResponse sr = cloudClient->SubInfo(sublist);
            if (sr == ServerResponse::TokenExpired) {
                mLogger.LogInfo() << "[Sub] Token Expired!";
                RefreshToken();
            } else if (sr == ServerResponse::Error) {
                mLogger.LogInfo() << "[Sub] Failed!";
            } else {
                mLogger.LogInfo() << "[Sub] Success!";
                notifySubscriptionInfo(sublist);
            }
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    });
    sub_thread.detach();
}

void
SubscriptionManager::RefreshToken() {
    cloudClient->RefreshToken();
}

bool
SubscriptionManager::requestOtaProcess(const eevp::type::String& accessToken,
                                       const eevp::type::String& refreshToken,
                                       const eevp::ota::subscription::type::downloadPackageInfoList& downloadPackageInfoList) {
    mLogger.LogInfo() << __func__<< downloadPackageInfoList.size();

    return otaSubscriptionProxyImpl->requestOtaProcess(accessToken, refreshToken, downloadPackageInfoList);
}

void
SubscriptionManager::notifySubscriptionInfo(std::vector<eevp::subscription::type::SubscriptionInfo> subscriptionList) {
    mLogger.LogInfo() << __func__ << subscriptionList.size();

    for (const auto& info : subscriptionList) {
        eevp::subscription::type::SubscriptionInfo item = info;
        subscriptionManagementSkeletonImpl->sendNotifySubscriptionInfo(item);
    }
}

bool
SubscriptionManager::getSubscriptionInfo(const eevp::type::String& appName) {
    mLogger.LogInfo() << __func__;

    auto isSub = std::find_if(sublist.begin(), sublist.end(),
                            [&](const eevp::subscription::type::SubscriptionInfo& info) {
                                return info.appName == appName;
                            });

    if (isSub != sublist.end()) {
        return isSub->isSubscription;
    }

    return false;
}

bool
SubscriptionManager::startOtaSubscriptionProxy() {
    otaSubscriptionProxyImpl = std::make_shared<eevp::subscription::OtaSubscriptionProxyImpl>();
    auto otaSubscriptionListener = std::make_shared<OtaSubscriptionListener>(this);
    otaSubscriptionProxyImpl->setEventListener(otaSubscriptionListener);
    otaSubscriptionProxyImpl->init();
    return true;
}

bool
SubscriptionManager::startSubscriptionManagementStub() {
    ara::core::InstanceSpecifier specifier("SubscriptionManager/AA/PPortSubscriptionManagement");
    subscriptionManagementSkeletonImpl = std::make_shared<eevp::subscription::SubscriptionManagementSkeletonImpl>(specifier);
    auto subscriptionManagementListener = std::make_shared<SubscriptionManagementListener>(this);
    subscriptionManagementSkeletonImpl->setEventListener(subscriptionManagementListener);
    subscriptionManagementSkeletonImpl->OfferService();
    return true;
}

bool
SubscriptionManager::startCloudClient() {
    cloudClient = std::make_shared<cloud::CloudClient>();

    if (cloudClient == nullptr) {
        return false;
    }

    cloudClient->init();
    return true;
}

} // namespace subscription
} // namespace eevp
