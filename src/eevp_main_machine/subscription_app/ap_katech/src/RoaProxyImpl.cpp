#include "RoaProxyImpl.h"

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::roa;

namespace eevp {
namespace control {
namespace roa {

RoaProxyImpl::RoaProxyImpl() :
        mProxy{nullptr},
        mFindHandle{nullptr},
        mHandle{},
        cvHandle{},
    mLogger(ara::log::CreateLogger("KATC", "ROA", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

RoaProxyImpl::~RoaProxyImpl() {
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

void
RoaProxyImpl::setEventListener(std::shared_ptr<IRoaListener> _listener) {
    listener = _listener;
}

bool
RoaProxyImpl::init() {
    mLogger.LogInfo() << __func__;

    ara::core::InstanceSpecifier specifier("KATECH/AA/RPortRoa");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandle);

    auto result = proxy::SoaRoaProxy::StartFindService(callback, specifier);
    
    if (cvHandle.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout) {
        return false;
    }

    if (!result.HasValue()) {
        mLogger.LogInfo() << "SoaRoaProxy StartFindService() Failed";
        return false;
    }

    return true;
}

bool
RoaProxyImpl::getSoaRoaDetectState(eevp::control::SoaRoaDetectState& soaRoaDetectState) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    auto future = mProxy->soaRoaDetectState.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            soaRoaDetectState = static_cast<eevp::control::SoaRoaDetectState>(result.Value());
            return true;
        } else {
            mLogger.LogError() << __func__ << ": Return error with " << result.Error().Message();
        }
    } else {
        mLogger.LogError() << "Timeout to call soaRoaDetectState's Getter";
    }

    return false;
}

bool
RoaProxyImpl::getSoaRoaDetectCount(uint8_t& soaRoaDetectCount) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    auto future = mProxy->soaRoaDetectCount.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            soaRoaDetectCount =result.Value();
            return true;
        } else {
            mLogger.LogError() << __func__ << ": Return error with " << result.Error().Message();
        }
    } else {
        mLogger.LogError() << "Timeout to call soaRoaDetectCount's Getter";
    }

    return false;
}

void
RoaProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaRoaProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;

    std::lock_guard<std::mutex> lock(mHandle);

    if (mProxy != nullptr) {
        UnsubscribeField();

        mFindHandle = nullptr;
        mProxy = nullptr;
    }

    if (container.empty()) {
        mProxy = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    mProxy = std::make_shared<proxy::SoaRoaProxy>(container.at(0));

    SubscribeSoaRoaDetectCount();

    cvHandle.notify_one();
}

void
RoaProxyImpl::SubscribeSoaRoaDetectCount() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->soaRoaDetectCount.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    auto result = mProxy->soaRoaDetectCount.SetReceiveHandler(std::bind(&RoaProxyImpl::cbSoaRoaDetectCount, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for cbSoaRoaDetectCount with " << result.Error().Message();
    }

    result = mProxy->soaRoaDetectCount.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for cbSoaRoaDetectCount with " << result.Error().Message();
    }
}

void
RoaProxyImpl::cbSoaRoaDetectCount() {
    mLogger.LogInfo() << __func__;

    std::uint8_t fieldValue;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->soaRoaDetectCount.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->soaRoaDetectCount.GetNewSamples([&](auto msg) {
        fieldValue = static_cast<std::uint8_t>(*msg);
        mLogger.LogInfo() << "cbSoaRctnStatus : " << fieldValue;
        if (listener != nullptr) {
            listener->notifySoaRoaDetectCount(fieldValue);
        }
    });
}
void
RoaProxyImpl::UnsubscribeField() {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) {
        return;
    }

    mProxy->soaRoaDetectState.Unsubscribe();
}


} /// namespace roa
} /// namespace control
} /// namespace eevp
