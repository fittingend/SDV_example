#include "SmartFilmProxyImpl.h"
#include <future> // future_status 사용을 위해 추가

namespace eevp {
namespace control {
namespace smartfilm {

SmartFilmProxyImpl::SmartFilmProxyImpl() :
    mProxy{nullptr},
    mFindHandle{nullptr},
    mLogger(ara::log::CreateLogger("TSMT", "SMARTFILM", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

SmartFilmProxyImpl::~SmartFilmProxyImpl() {
    if (mProxy && mFindHandle) {
        mProxy->StopFindService(*mFindHandle);
    }
}

bool SmartFilmProxyImpl::init() {
    mLogger.LogInfo() << __func__;
    ara::core::InstanceSpecifier specifier("SmokingMonitor/AA/RPort_SOA_SmartFilm");

    auto callback = [this](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandleMutex);
    auto result = proxy::SoaSmartFilmProxy::StartFindService(callback, specifier);
    
    if (!result.HasValue()) {
        mLogger.LogError() << "SmartFilmProxyImpl StartFindService() Failed";
        return false;
    }
    
    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(result.Value());

    if (mCv.wait_for(lock, std::chrono::milliseconds(2000)) == std::cv_status::timeout) {
        mLogger.LogError() << "SmartFilmProxyImpl service discovery timed out";
        return false;
    }

    return true;
}

void SmartFilmProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaSmartFilmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;
    if (container.empty()) {
        mLogger.LogWarn() << "SmartFilmProxyImpl service discovered but container is empty.";
        mProxy = nullptr;
        mCv.notify_one(); // Notify even if service not found to unblock init
        return;
    }

    mLogger.LogInfo() << "SmartFilmProxyImpl service found, creating proxy.";
    mProxy = std::make_shared<proxy::SoaSmartFilmProxy>(container.at(0));
    SubscribeSoaFilmOpacities();
    mCv.notify_one();
}

void SmartFilmProxyImpl::SubscribeSoaFilmOpacities() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        mLogger.LogError() << "SubscribeSoaFilmOpacities called with null proxy.";
        return;
    }

    mProxy->soaFilmOpacities.SetReceiveHandler(std::bind(&SmartFilmProxyImpl::cbSoaFilmOpacities, this));
    mProxy->soaFilmOpacities.Subscribe(1);
}


void SmartFilmProxyImpl::cbSoaFilmOpacities() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        return;
    }

    if (mProxy->soaFilmOpacities.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        mLogger.LogWarn() << "cbSoaFilmOpacities called but not subscribed.";
        return;
    }

    // mProxy->soaFilmOpacities.GetNewSamples([this](auto sample) {
    //     if (mListener) {
    //         mListener->notifyDmsStatus(*sample);
    //     }
    // });
}


// --- Method 구현 추가 ---
void SmartFilmProxyImpl::SetOpacity(const eevp::control::SoaFilmPos& pos, const std::uint8_t& opacity) {
    mLogger.LogInfo() << __func__ ;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetOpacity(pos, opacity);
    return;
}

void SmartFilmProxyImpl::SetAllOpacities(const std::uint8_t& opacity) {
    mLogger.LogInfo() << __func__ << opacity;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetAllOpacities(opacity);
    return;
}

void SmartFilmProxyImpl::SetAuto(const bool& on) {
    mLogger.LogInfo() << __func__ << on;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetAuto(on);
    return;
}                                                       

// --- Field 구현 추가 ---
bool SmartFilmProxyImpl::getSoaFilmOpacities(eevp::control::SoaFilmOpacityArray& status) {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) return false;
    auto future = mProxy->soaFilmOpacities.Get();
    if (future.wait_for(std::chrono::milliseconds(100)) == ara::core::future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            status = result.Value();
            return true;
        }
    }
    return false;
}

}
}
}