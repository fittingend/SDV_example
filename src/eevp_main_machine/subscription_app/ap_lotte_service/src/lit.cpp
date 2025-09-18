#include "lit.h"
#include "DmsProxyImpl.h"
#include "SmartFilmProxyImpl.h"
#include "SubscriptionManagementProxyImpl.h"
#include "ara/exec/execution_client.h"
#include <ctime>

namespace eevp {
namespace control {

const eevp::type::String LIT::mAppname = "LIT";
std::atomic_bool LIT::mRunning(false);

class SubscriptionManagementListener 
    : public eevp::subscription::service::ISubscriptionManagementListener {
public:
    explicit SubscriptionManagementListener(LIT* appInstance)
        : lit(appInstance) {}

    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& info) override
    {
        lit->notifySubscriptionInfo(info);
    }
private:
    LIT* lit;
};

LIT::LIT()
    : mLogger(ara::log::CreateLogger("LIT", "SWC", ara::log::LogLevel::kInfo))
    , mSubscription(true)
    , dmsProxyImpl{nullptr}
    , smartFilmProxyImpl{nullptr}
    , subscriptionManagementProxyImpl{nullptr}
    , mLitCloudProxy{nullptr}
{
    mLogger.LogInfo() << __func__;
    std::signal(SIGTERM, SignalHandler);
}
 
LIT::~LIT()
{
}

void LIT::SignalHandler(std::int32_t /*signal*/) {
    mRunning = false;
}
 
bool LIT::Initialize()
{
    mLogger.LogInfo() << "LIT::Initialize";
    transparencyMap[eevp::control::SoaFilmPos::kDS_FRONTREAR] = {false, initTransparency, 0};
    transparencyMap[eevp::control::SoaFilmPos::kAS_FRONTREAR] = {false, initTransparency, 0};
    transparencyMap[eevp::control::SoaFilmPos::kREAR_GLASS] = {false, initTransparency, 0};
    return true;
}
 
bool LIT::Start()
{
    mLogger.LogInfo() << "LIT::Start";
    mRunning = true;

    if(!Initialize()) return false;

    if(!setRunningState()) return false;
    if(!startSubscriptionManagementProxy()) return false;
    if(!startDmsProxy()) return false;
    if(!startSmartFilmProxy()) return false;

    mLogger.LogInfo() << "Starting LITCloudProxy...";
    mLitCloudProxy = std::make_shared<LITCloudProxy>();
    if (!mLitCloudProxy->init()) {
        mLogger.LogError() << "LITCloudProxy initialization failed.";
        return false;
    }
    
    mLogger.LogInfo() << "Checking required services status...";
    eevp::control::SoaDmsStatus dmsStatus;
    eevp::control::SoaDeviceIsNormal filmStatus;

    if (dmsProxyImpl && smartFilmProxyImpl && 
        dmsProxyImpl->getSoaDmsStatus(dmsStatus) && 
        smartFilmProxyImpl->getSoaFilmDeviceNormal(filmStatus)) {
         mLogger.LogInfo() << "DMS and SmartFilm services are ready.";
    } else {
        mLogger.LogError() << "Dms Status or SmartFilm Status Init FAILED. Exiting.";
        return false;
    }
    
    return true;
}
 
void LIT::Terminate()
{
    std::lock_guard<std::mutex> lock(mSubscriptionMutex);
    mRunning = false;
    mSubscription = false;
    mSubscriptionCv.notify_all();
    mLogger.LogInfo() << "App is terminating.";
}
 
void LIT::Run()
{
    mLogger.LogInfo() << "LIT::Run";

    while (mRunning) {
        std::unique_lock<std::mutex> lock(mSubscriptionMutex);
        
        if (!mSubscriptionCv.wait_for(lock, std::chrono::seconds(5),
            [&]() { return mSubscription || !mRunning; })) {
            mLogger.LogInfo() << "Still unsubscribed... waiting.";
            continue;
        }
        if (!mRunning) break;

        mLogger.LogInfo() << "App is subscribed. Starting main logic.";

        while (mSubscription && mRunning) {
            eevp::control::soaDmsDriverStatus driverStatus;
            if (dmsProxyImpl && dmsProxyImpl->getSoaDmsDriverStatus(driverStatus)) {

                // [추가] 클라우드 응답에 따라 로직을 처리했는지 여부를 판단하는 플래그
                bool isCloudControlled = false;

                // ########## START: 클라우드 연동 비지니스 로직 ##########
                if (mLitCloudProxy) {
                    // 1. 운전자의 시선 데이터를 클라우드 서버로 전송하고 응답을 받습니다.
                    auto cloudResponse = mLitCloudProxy->sendGazingData(driverStatus.gazingDir);

                    // 2. 서버로부터 유효한 응답(추천값)이 왔는지 확인합니다.
                    if (cloudResponse.has_value()) {
                        // 3. 응답이 유효하면, 클라우드 로직이 실행되었음을 플래그에 기록합니다.
                        isCloudControlled = true;
                        mLogger.LogInfo() << "[Cloud Logic] Message: " << cloudResponse.value().messageToDriver;

                        // 4. 서버가 추천한 투명도 값으로 실제 스마트 필름을 제어합니다.
                        if (smartFilmProxyImpl) {
                            // 예시: 모든 창문의 투명도를 서버 추천값으로 일괄 변경
                            smartFilmProxyImpl->requestSetOpacity(
                                eevp::control::SoaFilmPos::kALL, 
                                cloudResponse.value().recommendedOpacity
                            );
                        }
                        // 참고: 여기서 기존의 타이머 로직(transparencyMap)을 비활성화하거나
                        // 서버 응답에 따라 새로운 타이머를 설정하는 등 추가 로직 구현이 가능합니다.
                    }
                }
                // ########## END: 클라우드 연동 비지니스 로직 ##########


                // ########## START: 기존 단말 기반 비지니스 로직 ##########
                // 5. 클라우드 응답이 없었을 경우에만 기존의 단말 자체 판단 로직을 수행합니다.
                if (!isCloudControlled) {
                    // 기존 시선 방향에 따른 로직 처리
                    switch (driverStatus.gazingDir) {
                        case eevp::control::SoaDmsGazingDir::kLEFT_MIRROR:
                            if(smartFilmProxyImpl) {
                                smartFilmProxyImpl->requestSetOpacity(eevp::control::SoaFilmPos::kDS_FRONTREAR, 100);
                                transparencyMap[eevp::control::SoaFilmPos::kDS_FRONTREAR] = {true, 100, 3000}; // 3초 타이머
                            }
                            break;
                        case eevp::control::SoaDmsGazingDir::kRIGHT_MIRROR:
                            if(smartFilmProxyImpl) {
                                smartFilmProxyImpl->requestSetOpacity(eevp::control::SoaFilmPos::kAS_FRONTREAR, 100);
                                transparencyMap[eevp::control::SoaFilmPos::kAS_FRONTREAR] = {true, 100, 3000}; // 3초 타이머
                            }
                            break;
                        case eevp::control::SoaDmsGazingDir::kREAR_MIRROR:
                            if(smartFilmProxyImpl) {
                                smartFilmProxyImpl->requestSetOpacity(eevp::control::SoaFilmPos::kREAR_GLASS, 100);
                                transparencyMap[eevp::control::SoaFilmPos::kREAR_GLASS] = {true, 100, 3000}; // 3초 타이머
                            }
                            break;
                        default:
                            break;
                    }
                }
                // ########## END: 기존 단말 기반 비지니스 로직 ##########
            }

            // 타이머 감소 및 초기화 로직 (단말 기반 로직에 의해 활성화됨)
            for (auto& pair : transparencyMap) {
                if (pair.second.isChanged) {
                    pair.second.remainingTime -= 100;
                    if (pair.second.remainingTime <= 0) {
                        if (smartFilmProxyImpl) {
                            smartFilmProxyImpl->requestSetOpacity(pair.first, initTransparency);
                            pair.second = {false, initTransparency, 0};
                        }
                    }
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            if (!mSubscription) {
                mLogger.LogInfo() << "App is unsubscribed. Pausing logic.";
                break;
            }
        }
    }            
}

bool LIT::startDmsProxy() {
    mLogger.LogInfo() << __func__;
    dmsProxyImpl = std::make_shared<eevp::control::dms::DmsProxyImpl>();
    dmsProxyImpl->setEventListener(std::shared_ptr<LIT>(this, [](LIT*){}));
    dmsProxyImpl->init();
    return true;
}

void LIT::notifyDmsStatus(const eevp::control::SoaDmsStatus& status) {
    mLogger.LogInfo() << "[Async Callback] notifyDmsStatus Received!";
    std::lock_guard<std::mutex> lock(m_dmsMutex);
    m_dmsStatus.emplace(status);
}

bool LIT::startSmartFilmProxy() {
    mLogger.LogInfo() << __func__;
    smartFilmProxyImpl = std::make_shared<eevp::control::smartfilm::SmartFilmProxyImpl>();
    smartFilmProxyImpl->setEventListener(std::shared_ptr<LIT>(this, [](LIT*){}));
    smartFilmProxyImpl->init();
    return true;
}

void LIT::notifyFilmDeviceNormal(const eevp::control::SoaDeviceIsNormal& status) {
    mLogger.LogInfo() << "[Async Callback] notifyFilmDeviceNormal Received!";
    std::lock_guard<std::mutex> lock(m_smartfilmMutex);
    m_filmDeviceNormal.emplace(status);
}

bool 
LIT::startSubscriptionManagementProxy() {
    mLogger.LogInfo() << __func__;
    subscriptionManagementProxyImpl = std::make_shared<eevp::subscription::service::SubscriptionManagementProxyImpl>();
    auto subscriptionManagementListener = std::make_shared<SubscriptionManagementListener>(this);
    subscriptionManagementProxyImpl->setEventListener(subscriptionManagementListener);
    subscriptionManagementProxyImpl->init();
    return true;
}

/**
 * @brief SubscriptionManagement 서비스로부터 구독 상태 변경 이벤트를 수신하는 콜백 함수입니다.
 *        (ISubscriptionManagementListener 인터페이스의 구현부)
 * @param value 구독 정보 (앱 이름, 구독 여부)가 담긴 구조체
 */
void 
LIT::notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& value) {
    mLogger.LogInfo() << __func__;
    // 1. 이 알림이 본인 앱(LIT)에 대한 것인지 확인합니다.
    if (mAppname == value.appName) {
        {
            // 2. 앱의 현재 구독 상태(mSubscription)를 스레드에 안전하게 업데이트합니다.
            std::lock_guard<std::mutex> lock(mSubscriptionMutex);
            mSubscription = value.isSubscription;
        }

        // 3. 클라우드 프록시를 통해, 구독 상태가 변경되었음을 서버로 전송합니다.
        if (mLitCloudProxy) {
            mLitCloudProxy->sendSubscriptionEvent(value);
        }

        // 4. 구독 상태에 따라 Run() 메서드의 메인 루프를 제어합니다.
        if (value.isSubscription) {
            // 4-1. 구독이 시작되면, 대기 중인 Run() 루프를 깨워 로직을 다시 시작시킵니다.
            mLogger.LogInfo() << "App subscribed -> waking up logic";
            mSubscriptionCv.notify_all();
        } else {
            // 4-2. 구독이 해지되면, Run() 루프가 다음 주기부터 동작을 멈추게 됩니다.
            mLogger.LogInfo() << "App unsubscribed -> logic will pause";
        }
    }
}

void LIT::getSubscriptionInfo()
{
    mLogger.LogInfo() << "LIT::getSubscriptionInfo";
    subscriptionManagementProxyImpl->getSubscriptionInfo(mAppname, mSubscription);
}

bool
LIT::setRunningState() {
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (exec.HasValue()) {
        mLogger.LogInfo() << "LIT app in Running State";
    } else {
        mLogger.LogError() << exec.Error().Message();
        return false;
    }
    return true;
}
 
} // namespace control
} // namespace eevp
