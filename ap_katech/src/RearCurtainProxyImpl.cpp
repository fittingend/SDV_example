#include "RearCurtainProxyImpl.h"

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::rearcurtain;

namespace eevp {
namespace control {
namespace rearcurtain {

RearCurtainProxyImpl::RearCurtainProxyImpl() :
        mProxy{nullptr},
        mFindHandle{nullptr},
        mHandle{},
        cvHandle{},
    mLogger(ara::log::CreateLogger("KATC", "RCTN", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

RearCurtainProxyImpl::~RearCurtainProxyImpl() {
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

// void
// RearCurtainProxyImpl::setEventListener(std::shared_ptr<IRearCurtainListener> _listener) {
//     listener = _listener;
// }

bool
RearCurtainProxyImpl::init() {
    mLogger.LogInfo() << __func__;

    ara::core::InstanceSpecifier specifier("KATECH/AA/RPortRearCurtain");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandle);

    auto result = proxy::SoaRcurtainProxy::StartFindService(callback, specifier);

    if (cvHandle.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout) {
        return false;
    }

    if (!result.HasValue()) {
        mLogger.LogInfo() << "SoaRcurtainProxy StartFindService() Failed";
        return false;
    }

    return true;
}

eevp::control::SoaErrorState
RearCurtainProxyImpl::requestRearCurtainOperation(const eevp::control::SoaRctnMotorDir& motorDir) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return eevp::control::SoaErrorState::kERROR;
    }

    auto future = mProxy->RequestRearCurtainOperation(motorDir);
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            auto value = result.Value();
            return value.err;
        } else {
            mLogger.LogError() << __func__ << ": Return error with " << result.Error().Message();
        }
    } else {
        mLogger.LogError() << "Timeout to call RequestRearCurtainOperation";
    }

    return eevp::control::SoaErrorState::kERROR;

}

void
RearCurtainProxyImpl::requestRearCurtainPosition(const std::uint8_t& posPercentage) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->RequestRearCurtainPosition(posPercentage);
    return;
}

bool
RearCurtainProxyImpl::getSoaRctnStatus(eevp::control::SoaRctnStatus& soaRctnStatus) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return false;
    }

    auto future = mProxy->soaRctnStatus.Get();
    auto status = future.wait_for(std::chrono::milliseconds(10));
    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            soaRctnStatus = static_cast<eevp::control::SoaRctnStatus>(result.Value());
            return true;
        } else {
            mLogger.LogError() << __func__ << ": Return error with " << result.Error().Message();
        }
    } else {
        mLogger.LogError() << "Timeout to call soaRctnStatus's Getter";
    }

    return false;
}

void
RearCurtainProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaRcurtainProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;

    std::lock_guard<std::mutex> lock(mHandle);

    if (mProxy != nullptr) {
        mFindHandle = nullptr;
        mProxy = nullptr;
    }

    if (container.empty()) {
        mProxy = nullptr;
        return;
    }

    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    mProxy = std::make_shared<proxy::SoaRcurtainProxy>(container.at(0));
    cvHandle.notify_one();
}

} /// namespace rearcurtain
} /// namespace control
} /// namespace eevp
