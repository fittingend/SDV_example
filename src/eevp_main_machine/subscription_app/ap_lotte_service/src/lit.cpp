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
    // 윈도우 정보 초기화
    transparencyMap[eevp::control::SoaFilmPos::kDS_FRONTREAR] = {false, initTransparency, 0};
    transparencyMap[eevp::control::SoaFilmPos::kAS_FRONTREAR] = {false, initTransparency, 0};
    transparencyMap[eevp::control::SoaFilmPos::kREAR_GLASS] = {false, initTransparency, 0};
    return true;
}
 
bool LIT::Start()
{
    mLogger.LogInfo() << "LIT::Start";
    mRunning = true;

    if(!Initialize()) return false; // Initialize() 호출 추가

    if(!setRunningState()) return false;
    if(!startSubscriptionManagementProxy()) return false;
    if(!startDmsProxy()) return false;
    if(!startSmartFilmProxy()) return false;
    
    // --- 서비스 상태 확인 로직 추가 ---
    mLogger.LogInfo() << "Checking required services status...";
    eevp::control::SoaDmsStatus dmsStatus;
    eevp::control::SoaDeviceIsNormal filmStatus;

    if (dmsProxyImpl && smartFilmProxyImpl && 
        dmsProxyImpl->getSoaDmsStatus(dmsStatus) && 
        smartFilmProxyImpl->getSoaFilmDeviceNormal(filmStatus)) {
         mLogger.LogInfo() << "DMS and SmartFilm services are ready.";
    } else {
        mLogger.LogError() << "Dms Status or SmartFilm Status Init FAILED. Exiting.";
        return false; // 시작 중단(test)
    }
    
    //Run();

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
                // 시선 방향에 따라 로직 처리
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

            // 타이머 감소 및 초기화 로직
            for (auto& pair : transparencyMap) {
                if (pair.second.isChanged) {
                    pair.second.remainingTime -= 100; // 100ms 감소
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

void 
LIT::notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& value) {
    mLogger.LogInfo() << __func__;
    if (mAppname == value.appName) {
        {
            std::lock_guard<std::mutex> lock(mSubscriptionMutex);
            mSubscription = value.isSubscription;
        }
        if (value.isSubscription) {
            mLogger.LogInfo() << "App subscribed → waking up logic";
            mSubscriptionCv.notify_all();
        } else {
            mLogger.LogInfo() << "App unsubscribed → logic will pause";
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