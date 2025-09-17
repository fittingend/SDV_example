#include "BmsInfoProxyImpl.h"

using namespace ara::core;
using namespace eevp;
using namespace eevp::bmsinfosrv;
// using namespace eevp::bmsinfosrv::service;

namespace eevp {
namespace bmsinfosrv {
// namespace proxy {

BmsInfoProxyImpl::BmsInfoProxyImpl() :
        mProxy{nullptr},
        mFindHandle{nullptr},
        mHandle{},
        cvHandle{},
    mLogger(ara::log::CreateLogger("TSMT", "SUBM", ara::log::LogLevel::kInfo)) {
    mLogger.LogInfo() << __func__;
}

BmsInfoProxyImpl::~BmsInfoProxyImpl() {
    if (mProxy) {
        mProxy->StopFindService(*mFindHandle);
        mProxy.reset();
    }
}

void
BmsInfoProxyImpl::setEventListener(std::shared_ptr<eevp::bmsinfo::service::IBmsInfoListener> _listener) {
    listener = _listener;
}

bool
BmsInfoProxyImpl::init() {
    mLogger.LogInfo() << __func__;

    mLogger.LogInfo() << "BmsInfo StartFindService() Start!!";
    // 어떤 서비스를 찾을지 지정 (InstanceSpecifier는 AUTOSAR ARA::COM에서 사용되는 서비스 식별자)
    ara::core::InstanceSpecifier specifier("BatteryMonitor/AA/RPort_BmsInfo");

    // 서비스가 발견되었을 때 호출되는 콜백 정의
    auto callback = [&](auto container, auto findHandle) {
        FindServiceCallback(container, findHandle);// 서비스 연결 시도
    };
    // FindService의 결과 기다릴 때 사용할 mutex lock
    std::unique_lock<std::mutex> lock(mHandle);

    // 비동기적으로 서비스 탐색 시작 (서비스 발견 시 callback 호출됨)
    auto result = proxy::BmsInfoSrvProxy::StartFindService(callback, specifier);
    
    // 일정 시간 동안 서비스 탐색 결과 대기
    if (cvHandle.wait_for(lock, std::chrono::milliseconds(1000)) == std::cv_status::timeout) {
        return false;
    }

    //log
    auto error = result.Error();
    mLogger.LogError() << "Error details: ErrorCode = " << error;
    //log
    
    // StartFindService 자체가 실패한 경우
    if (!result.HasValue()) {
        mLogger.LogInfo() << "BmsInfoSrvProxy StartFindService() Failed";
        return false;
    }

    return true;// 초기화 성공
}

void
BmsInfoProxyImpl::FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::bmsinfosrv::proxy::BmsInfoSrvProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle) {
    mLogger.LogInfo() << __func__;

    // 서비스 핸들 보호용 mutex lock
    std::lock_guard<std::mutex> lock(mHandle);
    
    // 이미 프록시가 존재한다면 기존 구독 해제 및 프록시 제거
    if (mProxy != nullptr) {
        UnsubscribeBmsInfo();// 기존 ems_BmsInfo 이벤트 핸들러 해제
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
    // 새로운 BmsInfoSrvProxy 생성
    mProxy = std::make_shared<proxy::BmsInfoSrvProxy>(container.at(0));

    // ems_BmsInfo 수신 시작 (리스너 등록)
    SubscribeBmsInfo();
    // 대기 중인 init() 측 조건 변수 깨움
    cvHandle.notify_one();
}

void
BmsInfoProxyImpl::SubscribeBmsInfo() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    // 콜백 함수 등록 (이벤트 수신 시 cbBmsInfo 호출되도록 설정)
    auto result = mProxy->ems_BmsInfo.SetReceiveHandler(std::bind(&BmsInfoProxyImpl::cbBmsInfo, this));
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to SetReceiveHandler for cbBmsInfo with " << result.Error().Message();
    }

    // 실제로 이벤트 구독 시작 (Queue 사이즈 10)
    result = mProxy->ems_BmsInfo.Subscribe(10);
    if (!result.HasValue()) {
        mLogger.LogWarn() << "Failed to Subscribe for cbBmsInfo with " << result.Error().Message();
    }
}

void
BmsInfoProxyImpl::cbBmsInfo() {
    mLogger.LogInfo() << __func__;

    if (mProxy == nullptr) {
        return;
    }

    mProxy->ems_BmsInfo.GetNewSamples([&](auto msg) {

        
        const auto& bmsInfo = *msg;
      

        // 외부에서 등록한 리스너가 있다면 전달
        if (listener != nullptr) {
            listener->ems_BmsInfo(bmsInfo);
        }
    });
}
void
BmsInfoProxyImpl::UnsubscribeBmsInfo() {
    mLogger.LogInfo() << __func__;
    if (mProxy == nullptr) {
        return;
    }

    mProxy->ems_BmsInfo.Unsubscribe();
}


// } /// namespace proxy
} /// namespace bmsInfo
} /// namespace eevp