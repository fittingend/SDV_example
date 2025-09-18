#include "MoodLampProxyImpl.h"

using namespace ara::core;
using namespace eevp::control;
using namespace eevp::control::moodlamp;

namespace eevp {
namespace control {
namespace moodlamp {

MoodLampProxyImpl::MoodLampProxyImpl() :
        mProxy{nullptr},
        mFindHandle{nullptr},
        mHandle{},
        cvHandle{},
    mLogger(ara::log::CreateLogger("TSMT", "RCTN", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

MoodLampProxyImpl::~MoodLampProxyImpl() {
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

// void
// MoodLampProxyImpl::setEventListener(std::shared_ptr<IMoodLampListener> _listener) {
//     listener = _listener;
// }

bool
MoodLampProxyImpl::init() {
    mLogger.LogInfo() << __func__;

    mLogger.LogInfo() << "SoaMlmProxy StartFindService() Start!!";
    ara::core::InstanceSpecifier specifier("BatteryMonitor/AA/RPort_SOA_Mlm");

    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);
    };

    std::unique_lock<std::mutex> lock(mHandle);

    auto result = proxy::SoaMlmProxy::StartFindService(callback, specifier);

    if (cvHandle.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout) {
        return false;
    }

    if (!result.HasValue()) {
        mLogger.LogInfo() << "SoaMlmProxy StartFindService() Failed";
        return false;
    }

    return true;
}

void
MoodLampProxyImpl::RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->RequestMlmSetRgbColor(colorTableIndex);
    return;
}

void
MoodLampProxyImpl::RequestMlmSetBrightness(const std::uint16_t& brightness) {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->RequestMlmSetBrightness(brightness);
    return;
}

void
MoodLampProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<proxy::SoaMlmProxy::HandleType> container,
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
    mProxy = std::make_shared<proxy::SoaMlmProxy>(container.at(0));
    cvHandle.notify_one();
}

} /// namespace moodlamp
} /// namespace control
} /// namespace eevp
