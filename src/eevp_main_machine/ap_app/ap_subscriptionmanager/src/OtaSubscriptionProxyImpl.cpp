#include "OtaSubscriptionProxyImpl.h"

using namespace eevp::subscription;
using namespace eevp::ota::subscription::type;
using namespace eevp::ota::subscription::service;

namespace eevp {
namespace subscription {

OtaSubscriptionProxyImpl::OtaSubscriptionProxyImpl() :
        mProxy{nullptr},
        mFindHandle{nullptr},
        mHandle{},
        cvHandle{},
    mLogger(ara::log::CreateLogger("SUBM", "OTA", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

OtaSubscriptionProxyImpl::~OtaSubscriptionProxyImpl() {
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

void
OtaSubscriptionProxyImpl::setEventListener(std::shared_ptr<IOtaSubscriptionListener> _listener) {
    listener = _listener;
}

bool
OtaSubscriptionProxyImpl::init() {
    mLogger.LogInfo() << __func__;

    ara::core::InstanceSpecifier specifier("SubscriptionManager/AA/RPortOtaSubscription");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandle);

    auto result = proxy::OtaSubscriptionProxy::StartFindService(callback, specifier);

    if (cvHandle.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout) {
        return false;
    }

    if (!result.HasValue()) {
        mLogger.LogInfo() << "OtaSubscriptionProxy StartFindService() Failed";
        return false;
    }

    return true;
}

bool
OtaSubscriptionProxyImpl::requestAppInstall(const eevp::type::String& serviceName) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    mProxy->requestAppInstall(serviceName);
    return true;

}

bool
OtaSubscriptionProxyImpl::requestAppUpdate(const eevp::type::String& serviceName) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    mProxy->requestAppUpdate(serviceName);
    return true;

}

bool
OtaSubscriptionProxyImpl::requestAppRollback() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    mProxy->requestAppRollback();
    return true;
}

bool
OtaSubscriptionProxyImpl::requestOtaProcess(const eevp::type::String& accessToken,
                                            const eevp::type::String& refreshToken,
                                            const eevp::ota::subscription::type::downloadPackageInfoList& downloadPackageInfoList) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    mProxy->requestOtaProcess(accessToken, refreshToken, downloadPackageInfoList);
    return true;
}

void
OtaSubscriptionProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::OtaSubscriptionProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;

    std::lock_guard<std::mutex> lock(mHandle);

    if (mProxy != nullptr) {
        UnsubscribeNoitfyAppEvent();
        UnsubscribeNotifyRollbackComplete();
        UnsubscribeNotifyUpdatableApp();
        UnsubscribeNotifyUpdateComplete();
        UnsubcribeNotifySwClusterInfo();
        UnsubscribeNotifyResultInfo();

        mFindHandle = nullptr;
        mProxy = nullptr;
    }

    if (container.empty()) {
        mLogger.LogWarn() << __func__ << "container empty";
        mProxy = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    mProxy = std::make_shared<proxy::OtaSubscriptionProxy>(container.at(0));

    SubscribeNoitfyAppEvent();
    SubscribeNotifyRollbackComplete();
    SubscribeNotifyUpdatableApp();
    SubscribeNotifyUpdateComplete();
    SubscribeNotifySwClusterInfo();
    SubscribeNotifyResultInfo();

    cvHandle.notify_one();
}

void
OtaSubscriptionProxyImpl::SubscribeNoitfyAppEvent() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyAppEvent.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->notifyAppEvent.SetReceiveHandler(std::bind(&OtaSubscriptionProxyImpl::cbAppEvent, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for notifyAppEvent with " << result.Error().Message();
    }

    result = mProxy->notifyAppEvent.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for notifyAppEvent with " << result.Error().Message();
    }
}

void
OtaSubscriptionProxyImpl::SubscribeNotifyRollbackComplete() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyRollbackComplete.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->notifyRollbackComplete.SetReceiveHandler(std::bind(&OtaSubscriptionProxyImpl::cbRollbackComplete, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for notifyRollbackComplete with " << result.Error().Message();
    }

    result = mProxy->notifyRollbackComplete.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for notifyRollbackComplete with " << result.Error().Message();
    }
}

void
OtaSubscriptionProxyImpl::SubscribeNotifyUpdatableApp() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyUpdatableApp.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->notifyUpdatableApp.SetReceiveHandler(std::bind(&OtaSubscriptionProxyImpl::cbUpdatableApp, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for notifyUpdatableApp with " << result.Error().Message();
    }

    result = mProxy->notifyUpdatableApp.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for notifyUpdatableApp with " << result.Error().Message();
    }
}

void
OtaSubscriptionProxyImpl::SubscribeNotifyUpdateComplete() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyUpdateComplete.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->notifyUpdateComplete.SetReceiveHandler(std::bind(&OtaSubscriptionProxyImpl::cbUpdateComplete, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for notifyUpdateComplete with " << result.Error().Message();
    }

    result = mProxy->notifyUpdateComplete.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for notifyUpdateComplete with " << result.Error().Message();
    }
}

void
OtaSubscriptionProxyImpl::SubscribeNotifySwClusterInfo() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifySwClusterInfo.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->notifySwClusterInfo.SetReceiveHandler(std::bind(&OtaSubscriptionProxyImpl::cbSwClusterInfo, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for notifySwClusterInfo with " << result.Error().Message();
    }

    result = mProxy->notifySwClusterInfo.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for notifySwClusterInfo with " << result.Error().Message();
    }
}

void
OtaSubscriptionProxyImpl::SubscribeNotifyResultInfo() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyResultInfo.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->notifyResultInfo.SetReceiveHandler(std::bind(&OtaSubscriptionProxyImpl::cbResultInfo, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for notifyResultInfo with " << result.Error().Message();
    }

    result = mProxy->notifyResultInfo.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for notifyResultInfo with " << result.Error().Message();
    }
}

void
OtaSubscriptionProxyImpl::UnsubscribeNoitfyAppEvent() {
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifyAppEvent.Unsubscribe();
}

void
OtaSubscriptionProxyImpl::UnsubscribeNotifyRollbackComplete() {
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifyRollbackComplete.Unsubscribe();
}

void
OtaSubscriptionProxyImpl::UnsubscribeNotifyUpdatableApp() {
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifyUpdatableApp.Unsubscribe();
}

void
OtaSubscriptionProxyImpl::UnsubscribeNotifyUpdateComplete() {
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifyUpdateComplete.Unsubscribe();
}

void
OtaSubscriptionProxyImpl::UnsubcribeNotifySwClusterInfo() {
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifySwClusterInfo.Unsubscribe();
}

void
OtaSubscriptionProxyImpl::UnsubscribeNotifyResultInfo() {
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifyResultInfo.Unsubscribe();
}

void
OtaSubscriptionProxyImpl::cbAppEvent() {
    mLogger.LogInfo() << "cbAppEvent";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyAppEvent.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifyAppEvent.GetNewSamples([&](auto noti) {
        requestAppInfo = static_cast<RequestAppInfo>(*noti);
        if (listener != nullptr) {
            listener->notifyAppEvent(requestAppInfo);
        }
    });
}

void
OtaSubscriptionProxyImpl::cbRollbackComplete() {
    mLogger.LogInfo() << "cbRollbackComplete";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyAppEvent.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifyRollbackComplete.GetNewSamples([&](auto noti) {
        retVal = static_cast<bool>(*noti);
        if (listener != nullptr) {
            listener->notifyRollbackComplete(retVal);
        }
    });
}

void
OtaSubscriptionProxyImpl::cbUpdatableApp() {
    mLogger.LogInfo() << "cbUpdatableApp";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyAppEvent.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifyUpdatableApp.GetNewSamples([&](auto noti) {
        updatableAppInfo = static_cast<UpdatableAppInfo>(*noti);
        if (listener != nullptr) {
            listener->notifyUpdatableApp(updatableAppInfo);
        }
    });
}

void
OtaSubscriptionProxyImpl::cbUpdateComplete() {
    mLogger.LogInfo() << "cbUpdateComplete";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyAppEvent.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifyUpdateComplete.GetNewSamples([&](auto noti) {
        retVal = static_cast<bool>(*noti);
        if (listener != nullptr) {
            listener->notifyUpdateComplete(retVal);
        }
    });
}

void
OtaSubscriptionProxyImpl::cbSwClusterInfo() {
    mLogger.LogInfo() << "cbSwClusterInfo";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifySwClusterInfo.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifySwClusterInfo.GetNewSamples([&](auto noti) {
        eevp::ota::subscription::type::swClusterInfoList list = static_cast<eevp::ota::subscription::type::swClusterInfoList>(*noti);
        if (listener != nullptr) {
            listener->notifySwClusterInfo(list);
        }
    });
}

void
OtaSubscriptionProxyImpl::cbResultInfo() {
    mLogger.LogInfo() << "cbResultInfo";

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifyResultInfo.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifyResultInfo.GetNewSamples([&](auto noti) {
        eevp::ota::subscription::type::resultInfoList list = static_cast<eevp::ota::subscription::type::resultInfoList>(*noti);
        if (listener != nullptr) {
            listener->notifyResultInfo(list);
        }
    });
}

} /// namespace subscription
} /// namespace eevp
