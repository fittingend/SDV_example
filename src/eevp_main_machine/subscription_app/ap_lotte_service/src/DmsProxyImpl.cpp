#include "DmsProxyImpl.h"
#include <future> // future_status 사용을 위해 추가
#include <string>

namespace eevp {
namespace control {
namespace dms {

// ... (이전 구현은 동일) ...

DmsProxyImpl::DmsProxyImpl() :
    mProxy{nullptr},
    mFindHandle{nullptr},
    mLogger(ara::log::CreateLogger("LIT", "LDMP")) {
    mLogger.LogInfo() << "DmsProxyImpl constructor";
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
    ara::core::InstanceSpecifier specifier("LIT/AA/RPortSoaDms");

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
    SubscribeSoaDmsStatus();
    mCv.notify_one();
}

void DmsProxyImpl::SubscribeSoaDmsStatus() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        mLogger.LogError() << "SubscribeSoaDmsStatus called with null proxy.";
        return;
    }

    mProxy->soaDmsStatus.SetReceiveHandler(std::bind(&DmsProxyImpl::cbSoaDmsStatus, this));
    mProxy->soaDmsStatus.Subscribe(1);
}


void DmsProxyImpl::cbSoaDmsStatus() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) {
        return;
    }

    if (mProxy->soaDmsStatus.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        mLogger.LogWarn() << "cbSoaDmsStatus called but not subscribed.";
        return;
    }

    mProxy->soaDmsStatus.GetNewSamples([this](auto sample) {
        if (mListener) {
            mListener->notifyDmsStatus(*sample);
        }
    });
}

// --- Getter 구현 추가 ---
bool DmsProxyImpl::getSoaDmsStatus(eevp::control::SoaDmsStatus& status) {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) {
        mLogger.LogError() << "getSoaDmsStatus called with null proxy.";
        return false;
    }

    auto future = mProxy->soaDmsStatus.Get();
    auto future_status = future.wait_for(std::chrono::milliseconds(100));

            if (future_status == ara::core::future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            status = result.Value();
            return true;
        } else {
            mLogger.LogError() << "getSoaDmsStatus getter returned error: " << result.Error().Message();
        }
    } else {
        mLogger.LogError() << "getSoaDmsStatus getter timed out.";
    }
    return false;
}

bool DmsProxyImpl::getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status) {
    if (mProxy == nullptr) return false;
    auto future = mProxy->soaDmsDriverStatus.Get();
    if (future.wait_for(std::chrono::milliseconds(100)) == ara::core::future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) {
            switch(result.Value().gazingDir){
                case eevp::control::SoaDmsGazingDir::kFRONT : mLogger.LogInfo() << "result Value : kFRONT"; break;
                case eevp::control::SoaDmsGazingDir::kFRONT_LEFT : mLogger.LogInfo() << "result Value : kFRONT_LEFT"; break;
                case eevp::control::SoaDmsGazingDir::kFRONT_RIGHT : mLogger.LogInfo() << "result Value : kFRONT_RIGHT"; break;
                case eevp::control::SoaDmsGazingDir::kREAR_MIRROR : mLogger.LogInfo() << "result Value : kREAR_MIRROR"; break;
                case eevp::control::SoaDmsGazingDir::kLEFT_MIRROR: mLogger.LogInfo() << "result Value : kLEFT_MIRROR"; break;
                case eevp::control::SoaDmsGazingDir::kRIGHT_MIRROR : mLogger.LogInfo() << "result Value : kRIGHT_MIRROR"; break;
                default : break;
            }
            //std::string str_num = std::to_string(result.Value().gazingDir)
            //mLogger.LogInfo() << "result Value : " << str_num;
            status = result.Value();
            return true;
        }
    }
    return false;
}


void DmsProxyImpl::UnsubscribeField() {
    mLogger.LogInfo() << __func__;
    if (!mProxy) return;
    mProxy->soaDmsStatus.Unsubscribe();
}

} // namespace dms
} // namespace control
} // namespace eevp
