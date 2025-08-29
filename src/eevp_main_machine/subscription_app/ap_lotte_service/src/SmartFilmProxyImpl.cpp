#include "SmartFilmProxyImpl.h"
#include <future>

namespace eevp {
namespace control {
namespace smartfilm {

SmartFilmProxyImpl::SmartFilmProxyImpl() :
    mProxy{nullptr},
    mFindHandle{nullptr},
    mLogger(ara::log::CreateLogger("LIT_SF", "SmartFilmProxyImpl")) {
}

SmartFilmProxyImpl::~SmartFilmProxyImpl() {
    if (mProxy && mFindHandle) {
        mProxy->StopFindService(*mFindHandle);
    }
}

void SmartFilmProxyImpl::setEventListener(std::shared_ptr<ISmartFilmListener> listener) {
    mListener = listener;
}

bool SmartFilmProxyImpl::init() {
    ara::core::InstanceSpecifier specifier("LIT/AA/RPortSoaSmartFilm");
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
    if (container.empty()) {
        mProxy = nullptr;
        mCv.notify_one();
        return;
    }
    mProxy = std::make_shared<proxy::SoaSmartFilmProxy>(container.at(0));
    SubscribeSoaFilmDeviceNormal();
    mCv.notify_one();
}

void SmartFilmProxyImpl::SubscribeSoaFilmDeviceNormal() {
    if (!mProxy) return;
    mProxy->soaFilmDeviceNormal.SetReceiveHandler(std::bind(&SmartFilmProxyImpl::cbSoaFilmDeviceNormal, this));
    mProxy->soaFilmDeviceNormal.Subscribe(1);
}

void SmartFilmProxyImpl::cbSoaFilmDeviceNormal() {
    if (!mProxy || !mListener) return;
    if (mProxy->soaFilmDeviceNormal.GetSubscriptionState() == ara::com::SubscriptionState::kSubscribed) {
        mProxy->soaFilmDeviceNormal.GetNewSamples([this](auto sample) {
            mListener->notifyFilmDeviceNormal(*sample);
        });
    }
}

bool SmartFilmProxyImpl::getSoaFilmDeviceNormal(eevp::control::SoaDeviceIsNormal& status) {
    if (mProxy == nullptr) return false;
    auto future = mProxy->soaFilmDeviceNormal.Get();
    if (future.wait_for(std::chrono::milliseconds(100)) == ara::core::future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            status = result.Value();
            return true;
        }
    }
    return false;
}

void SmartFilmProxyImpl::requestSetOpacity(eevp::control::SoaFilmPos pos, std::uint8_t opacity) {
    if (!mProxy) return;
    mProxy->SetOpacity(pos, opacity);
}

void SmartFilmProxyImpl::UnsubscribeField() {
    if (!mProxy) return;
    mProxy->soaFilmDeviceNormal.Unsubscribe();
}

} // namespace smartfilm
} // namespace control
} // namespace eevp
