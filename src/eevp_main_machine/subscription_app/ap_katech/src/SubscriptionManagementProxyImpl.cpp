#include "SubscriptionManagementProxyImpl.h"

using namespace ara::core;
using namespace eevp::subscription;
using namespace eevp::subscription::service;

namespace eevp {
namespace subscription {
namespace service {

SubscriptionManagementProxyImpl::SubscriptionManagementProxyImpl() :
        mProxy{nullptr},
        mFindHandle{nullptr},
        mHandle{},
        cvHandle{},
    mLogger(ara::log::CreateLogger("KATC", "SUBM", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

SubscriptionManagementProxyImpl::~SubscriptionManagementProxyImpl() {
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

void
SubscriptionManagementProxyImpl::setEventListener(std::shared_ptr<ISubscriptionManagementListener> _listener) {
    listener = _listener;
}

bool
SubscriptionManagementProxyImpl::init() {
    mLogger.LogInfo() << __func__;

    // 어떤 서비스를 찾을지 지정 (InstanceSpecifier는 AUTOSAR ARA::COM에서 사용되는 서비스 식별자)
    ara::core::InstanceSpecifier specifier("KATECH/AA/RPortSubscriptionManagement");

    // 서비스가 발견되었을 때 호출되는 콜백 정의
    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);// 서비스 연결 시도
    };
    // FindService의 결과 기다릴 때 사용할 mutex lock
    std::unique_lock<std::mutex> lock(mHandle);

    // 비동기적으로 서비스 탐색 시작 (서비스 발견 시 callback 호출됨)
    auto result = proxy::SubscriptionManagementProxy::StartFindService(callback, specifier);
    
    // 일정 시간 동안 서비스 탐색 결과 대기
    if (cvHandle.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout) {
        return false;
    }
    // StartFindService 자체가 실패한 경우
    if (!result.HasValue()) {
        mLogger.LogInfo() << "SubscriptionManagementProxy StartFindService() Failed";
        return false;
    }

    return true;// 초기화 성공
}

bool 
SubscriptionManagementProxyImpl::getSubscriptionInfo(eevp::type::String appName, bool& isSubscription) {
    mLogger.LogInfo() << "SubscriptionManagementProxyImpl::getSubscriptionInfo";

    if (!mProxy) {
        return false;
    }

    auto future = mProxy->getSubscriptionInfo(appName);
    auto status = future.wait_for(std::chrono::milliseconds(200));

    if (status == future_status::ready) {
        auto result = future.GetResult();
        if (result.HasValue()) 
        {
            isSubscription = result.Value().isSubscription;
            mLogger.LogInfo() << "App [" << appName << "] is "
                          << (isSubscription ? "subscribed" : "unsubscribed");
            return true;
        }
        else 
        {
            mLogger.LogError() << "getSubscriptionInfo() returned error: "
                            << result.Error().Message();  // optional
            return false;
        }
    }
}

void
SubscriptionManagementProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::subscription::service::proxy::SubscriptionManagementProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;

    // 서비스 핸들 보호용 mutex lock
    std::lock_guard<std::mutex> lock(mHandle);
    
    // 이미 프록시가 존재한다면 기존 구독 해제 및 프록시 제거
    if (mProxy != nullptr) {
        UnsubscribeSubscriptionInfo();// 기존 notifySubscriptionInfo 이벤트 핸들러 해제
        mFindHandle = nullptr;// 이전 핸들 제거
        mProxy = nullptr;// 프록시 포인터 제거
    }
    // 새로 받은 container가 비어있으면 → 유효한 서비스 없음 → 초기화 실패 처리
    if (container.empty()) {
        mProxy = nullptr;
        return;
    }
    // 새로 발견한 서비스 핸들 저장
    mFindHandle = std::make_shared<ara::com::FindServiceHandle>(findHandle);
    // 새로운 SubscriptionManagementProxy 생성
    mProxy = std::make_shared<proxy::SubscriptionManagementProxy>(container.at(0));

    // notifySubscriptionInfo 수신 시작 (리스너 등록)
    SubscribeSubscriptionInfo();
    // 대기 중인 init() 측 조건 변수 깨움
    cvHandle.notify_one();
}

void
SubscriptionManagementProxyImpl::SubscribeSubscriptionInfo() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifySubscriptionInfo.GetSubscriptionState() != ara::com::SubscriptionState::kNotSubscribed) {
        return;
    }

    // 콜백 함수 등록 (이벤트 수신 시 cbSubscriptionInfo 호출되도록 설정)
    auto result = mProxy->notifySubscriptionInfo.SetReceiveHandler(std::bind(&SubscriptionManagementProxyImpl::cbSubscriptionInfo, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for cbSubscriptionInfo with " << result.Error().Message();
    }

    // 실제로 이벤트 구독 시작 (Queue 사이즈 10)
    result = mProxy->notifySubscriptionInfo.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for cbSubscriptionInfo with " << result.Error().Message();
    }
}

void
SubscriptionManagementProxyImpl::cbSubscriptionInfo() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    if (mProxy->notifySubscriptionInfo.GetSubscriptionState() != ara::com::SubscriptionState::kSubscribed) {
        return;
    }

    mProxy->notifySubscriptionInfo.GetNewSamples([&](auto msg) {
        const auto& subscriptionInfo = *msg;
        mLogger.LogInfo() << "cbSubscriptionInfo : appName = " << subscriptionInfo.appName
                        << ", isSubscription = " << (subscriptionInfo.isSubscription ? "true" : "false");
        // 외부에서 등록한 리스너가 있다면 전달 (예: KATECH::notifySubscriptionInfo 호출됨)
        if (listener != nullptr) {
            listener->notifySubscriptionInfo(subscriptionInfo);
        }
    });
}
void
SubscriptionManagementProxyImpl::UnsubscribeSubscriptionInfo() {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) {
        return;
    }

    mProxy->notifySubscriptionInfo.Unsubscribe();
}


} /// namespace roa
} /// namespace control
} /// namespace eevp
