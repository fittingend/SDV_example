#include "DmsProxyImpl.h"
#include <future> // future_status 사용을 위해 추가

namespace eevp {
namespace control {
namespace dms {

// ... (이전 구현은 동일) ...

DmsProxyImpl::DmsProxyImpl() :
    mProxy{nullptr},
    mFindHandle{nullptr},
    mLogger(ara::log::CreateLogger("TSMT", "DMS", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

DmsProxyImpl::~DmsProxyImpl() {
    if (mProxy && mFindHandle) {
        mProxy->StopFindService(*mFindHandle);
    }
}

void DmsProxyImpl::setEventListener(std::shared_ptr<IDmsListener> listener) {
    mListener = listener;
}

bool DmsProxyImpl::init() {
    mLogger.LogInfo() << __func__;
    ara::core::InstanceSpecifier specifier("SmokingMonitor/AA/RPort_SOA_DMS");

    auto callback = [this](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandleMutex);
    auto result = proxy::SoaDmsProxy::StartFindService(callback, specifier);
    
    if (!result.HasValue()) {
        mLogger.LogError() << "DmsProxyImpl StartFindService() Failed";
        return false;
    }
    
    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(result.Value());

    if (mCv.wait_for(lock, std::chrono::milliseconds(2000)) == std::cv_status::timeout) {
        mLogger.LogError() << "DmsProxyImpl service discovery timed out";
        return false;
    }

    return true;
}

void DmsProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaDmsProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;
    if (container.empty()) {
        mLogger.LogWarn() << "DmsProxyImpl service discovered but container is empty.";
        mProxy = nullptr;
        mCv.notify_one(); // Notify even if service not found to unblock init
        return;
    }

    mLogger.LogInfo() << "DmsProxyImpl service found, creating proxy.";
    mProxy = std::make_shared<proxy::SoaDmsProxy>(container.at(0));
    SubscribeSoaDmsDriverStatus();
    mCv.notify_one();
}

void DmsProxyImpl::SubscribeSoaDmsDriverStatus() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        mLogger.LogError() << "SubscribeSoaDmsDriverStatus called with null proxy.";
        return;
    }

    mProxy->soaDmsDriverStatus.SetReceiveHandler(std::bind(&DmsProxyImpl::cbSoaDmsDriverStatus, this));
    mProxy->soaDmsDriverStatus.Subscribe(1);
}


void DmsProxyImpl::cbSoaDmsDriverStatus() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        return;
    }

    if (mProxy->soaDmsDriverStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        mLogger.LogWarn() << "cbSoaDmsDriverStatus called but not subscribed.";
        return;
    }

    // mProxy->soaDmsDriverStatus.GetNewSamples([this](auto sample) {
    //     if (mListener) {
    //         mListener->notifyDmsStatus(*sample);
    //     }
    // });
}

// --- Method 구현 추가 ---
void DmsProxyImpl::SetDmsPower(const bool& on) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetDmsPower(on);
    return;
}



// void DmsProxyImpl::getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status) {
//     mLogger.LogInfo() << __func__;
//     // if (mProxy == nullptr) {
//     //     return;
//     // }

//     mProxy->soaDmsDriverStatus.Get();

//     return;
// }

bool DmsProxyImpl::getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status) {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) return false;
    auto future = mProxy->soaDmsDriverStatus.Get();
    if (future.wait_for(std::chrono::milliseconds(100)) == ara::core::future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            status = result.Value();
            return true;
        }
    }
    return false;
}


void DmsProxyImpl::UnsubscribeField() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) return;
    mProxy->soaDmsDriverStatus.Unsubscribe();
}

} // namespace dms
} // namespace control
} // namespace eevp
