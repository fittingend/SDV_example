#include "HvacProxyImpl.h"
#include <future> // future_status 사용을 위해 추가

namespace eevp {
namespace control {
namespace hvac {

HvacProxyImpl::HvacProxyImpl() :
    mProxy{nullptr},
    mFindHandle{nullptr},
    mLogger(ara::log::CreateLogger("TSMT", "HVAC", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

HvacProxyImpl::~HvacProxyImpl() {
    if (mProxy && mFindHandle) {
        mProxy->StopFindService(*mFindHandle);
    }
}

bool HvacProxyImpl::init() {
    mLogger.LogInfo() << __func__;
    ara::core::InstanceSpecifier specifier("SmokingMonitor/AA/RPort_SOA_HVAC");

    auto callback = [this](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandleMutex);
    auto result = proxy::SoaHvacProxy::StartFindService(callback, specifier);
    
    if (!result.HasValue()) {
        mLogger.LogError() << "HvacProxyImpl StartFindService() Failed";
        return false;
    }
    
    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(result.Value());

    if (mCv.wait_for(lock, std::chrono::milliseconds(2000)) == std::cv_status::timeout) {
        mLogger.LogError() << "HvacProxyImpl service discovery timed out";
        return false;
    }

    return true;
}

void HvacProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaHvacProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;
    if (container.empty()) {
        mLogger.LogWarn() << "HvacProxyImpl service discovered but container is empty.";
        mProxy = nullptr;
        mCv.notify_one(); // Notify even if service not found to unblock init
        return;
    }

    mLogger.LogInfo() << "HvacProxyImpl service found, creating proxy.";
    mProxy = std::make_shared<proxy::SoaHvacProxy>(container.at(0));
    SubscribeSoaHvacSetting();
    mCv.notify_one();
}

void HvacProxyImpl::SubscribeSoaHvacSetting() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        mLogger.LogError() << "SubscribeSoaHvacSetting called with null proxy.";
        return;
    }

    mProxy->soaHvacSetting.SetReceiveHandler(std::bind(&HvacProxyImpl::cbSoaHvacSetting, this));
    mProxy->soaHvacSetting.Subscribe(1);
}


void HvacProxyImpl::cbSoaHvacSetting() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        return;
    }

    if (mProxy->soaHvacSetting.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        mLogger.LogWarn() << "cbSoaHvacSetting called but not subscribed.";
        return;
    }

    // mProxy->soaHvacSetting.GetNewSamples([this](auto sample) {
    //     if (mListener) {
    //         mListener->notifyDmsStatus(*sample);
    //     }
    // });
}


// --- Method 구현 추가 ---
void HvacProxyImpl::SetHvacPower(const bool& on) {
    mLogger.LogInfo() << __func__ << on;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetHvacPower(on);
    return;
}

void HvacProxyImpl::SetBlowingForce(const std::uint8_t& force) {
    mLogger.LogInfo() << __func__ << force;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetBlowingForce(force);
    return;
}

void HvacProxyImpl::SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetBlowingDirection(dir);
    return;
}

void HvacProxyImpl::SetAirSource(const eevp::control::SoaHvacAirSource& src) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->SetAirSource(src);
    return;
}

// --- Field 구현 추가 ---
bool HvacProxyImpl::getSoaHvacSetting(eevp::control::SoaHvacStatus& status) {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) return false;
    auto future = mProxy->soaHvacSetting.Get();
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