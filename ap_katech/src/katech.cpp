#include "katech.h"
#include "ara/exec/execution_client.h"
#include <ctime>

namespace eevp {
namespace control {

eevp::control::SoaRctnStatus soaRearCurtainStatus;
//eevp::subscription::type::SubscriptionInfo subscriptionInfo;
const eevp::type::String KATECH::mAppname = "KATECH";
std::atomic_bool KATECH::mRunning(false);

class RoaListener : public eevp::control::roa::IRoaListener {
public:
    RoaListener(KATECH* katech_app) : katech(katech_app) {}
    
    void notifySoaRoaDetectCount(std::uint8_t& value) {
        return katech->notifySoaRoaDetectCount(value);
    }
private:
    KATECH* katech;
};


/*
init()
 └─ StartFindService()
      └─ FindServiceCallback() → mProxy 생성 + SubscribeSubscriptionInfo()

         → SetReceiveHandler(cbSubscriptionInfo)
         → Subscribe(10)

             └─ notifySubscriptionInfo 이벤트 수신됨
                   └─ cbSubscriptionInfo() 호출됨
                         └─ listener->notifySubscriptionInfo(...) → KATECH::notifySubscriptionInfo()
*/
// Subscription 이벤트 받는 리스너
// 구독 정보 들어오면 KATECH에 전달함
class SubscriptionManagementListener 
    : public eevp::subscription::service::ISubscriptionManagementListener {
public:
    //SubscriptionManagementListener 객체 만들 때 KATECH 인스턴스의 포인터를 받아서 내부 멤버 변수 katech 으로 저장하는 것. 이후에 이 포인터를 통해 KATECH의 메서드를 호출
    explicit SubscriptionManagementListener(KATECH* appInstance)
        : katech(appInstance) {}

    // 구독 정보 들어오면 KATECH에 넘김
    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& info) override
    {
        // KATECH 객체의 함수에 구독 정보 전달
        katech->notifySubscriptionInfo(info);
    }

private:
    KATECH* katech;  // KATECH 애플리케이션 인스턴스에 대한 포인터
};

KATECH::KATECH() :
        mLogger(ara::log::CreateLogger("KATC", "SWC", ara::log::LogLevel::kInfo)),
        mSubscription(false),
        rearCurtainProxyImpl{nullptr},
        roaProxyImpl{nullptr},
        subscriptionManagementProxyImpl{nullptr}
{
    mLogger.LogInfo() << __func__;
    // std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
}


KATECH::~KATECH() {
}

void
KATECH::SignalHandler(std::int32_t /*signal*/) {
    mRunning = false;
}

bool KATECH::Start() {
    mLogger.LogInfo() << __func__;

    mRunning = true;

    if (!setRunningState()) return false;
    if (!startSubscriptionManagementProxy()) return false;
    if (!startRoaProxy()) return false;
    if (!startRearCurtainProxy()) return false;

    getSubscriptionInfo();  // 최초 구독 정보 조회하고 구독상태 상관없이 run 으로 무조건 진입 

    return true;
}

void KATECH::Run() {
    mLogger.LogInfo() << __func__;

    while (mRunning) {
        std::unique_lock<std::mutex> lock(mSubscriptionMutex);
        
        // 타임아웃 5초마다 깨어나서 상태 확인
        if (!mSubscriptionCv.wait_for(lock, std::chrono::seconds(5),
            [&]() { return mSubscription || !mRunning; })) {
            mLogger.LogInfo() << "Still unsubscribed... waiting.";
            continue;
        }
        if (!mRunning) break; // 종료 신호 받으면 나감

        mLogger.LogInfo() << "App is subscribed. Starting main logic.";

        while (mSubscription && mRunning) {
            // 실제 동작 수행
            eevp::control::SoaRoaDetectState soaRoaDetectState;
            std::uint8_t soaRoaDetectCount;

            getSoaRoaDetectState(soaRoaDetectState);
            getSoaRoaDetectCount(soaRoaDetectCount);

            mLogger.LogInfo() << "5 second periodic Event running";
            std::this_thread::sleep_for(std::chrono::seconds(5));

            // 중간에 구독이 끊기면 break
            if (!mSubscription) {
                mLogger.LogInfo() << "App is unsubscribed. Pausing logic.";
                break;
            }
        }
    }
}

void KATECH::Terminate() {
    std::lock_guard<std::mutex> lock(mSubscriptionMutex);
    mRunning = false;
    mSubscription = false;
    mSubscriptionCv.notify_all();  // Run 루프 탈출
    mLogger.LogInfo() << "App is terminating.";
}

bool
KATECH::setRunningState() {
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (exec.HasValue()) {
        mLogger.LogInfo() << "KATECH app in Running State";
    } else {
        mLogger.LogError() << exec.Error().Message();
        return false;
    }
    return true;
}

/// SubscriptionManagement Start

void KATECH::notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& value) {
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
            // `Run()` 내 루프가 이 상태 보고 break 됨
        }
    }
}
void 
KATECH::getSubscriptionInfo()
{
    mLogger.LogInfo() << "KATECH::getSubscriptionInfo";
    subscriptionManagementProxyImpl->getSubscriptionInfo(mAppname, mSubscription);
}
/// SubscriptionManagement End


/// ROA Start

void
KATECH::getSoaRoaDetectState(eevp::control::SoaRoaDetectState& soaRoaDetectState)
{
    roaProxyImpl->getSoaRoaDetectState(soaRoaDetectState);
    mLogger.LogInfo() << "[getSoaRoaDetectState]:" << static_cast<std::uint8_t>(soaRoaDetectState);
}

void
KATECH::getSoaRoaDetectCount(std::uint8_t& soaRoaDetectCount) 
{
    roaProxyImpl->getSoaRoaDetectCount(soaRoaDetectCount);
    mLogger.LogInfo() << "[getSoaRoaDetectCount]" << ":"
                      << soaRoaDetectCount;
}

/// ROA End


/// RearCurtain Start

void
KATECH::notifySoaRoaDetectCount(const std::uint8_t& detectCount) {
    mLogger.LogInfo() << __func__;
    mLogger.LogInfo() << "notified count:" << detectCount;
    
   if (checkRctnReady())
    {
        switch (detectCount)
        {
            case 0:
                if (soaRearCurtainStatus.curtainState == eevp::control::SoaRctnState::kFULLY_DOWN)
                {
                    // do nothing; rear curtain is already open
                    break;
                }
                else
                {
                    // when rear curtain is partially open or fully up 
                    bool result = requestRearCurtainOperation(eevp::control::SoaRctnMotorDir::kDOWN);
                    while (result)
                    {
                        mLogger.LogInfo() << "waiting for rear curtain operation to finish";
                        std::this_thread::sleep_for(std::chrono::seconds(RCTN_OP_TIME));
                        getSoaRctnStatus(soaRearCurtainStatus);
                        if (soaRearCurtainStatus.curtainState == eevp::control::SoaRctnState::kFULLY_DOWN)
                        {
                            mLogger.LogInfo() << "rear curtain operation finished";
                            break;
                        }
                    }
                    if (!result) 
                    {
                        mLogger.LogInfo() << "rear curtain operation unsuccessful";
                    }
                }
                break;
            
            case 1:
                if (soaRearCurtainStatus.curtainState == eevp::control::SoaRctnState::kFULLY_UP)
                {
                    // do nothing; rear curtain is already closed 
                    break;
                }
                else
                {
                    // when rear curtain is partially open or fully down 
                    bool result = requestRearCurtainOperation(eevp::control::SoaRctnMotorDir::kUP);
                    while (result)
                    {
                        mLogger.LogInfo() << "waiting for rear curtain operation to finish";
                        std::this_thread::sleep_for(std::chrono::seconds(RCTN_OP_TIME));
                        getSoaRctnStatus(soaRearCurtainStatus);
                        mLogger.LogInfo() << "waith" << static_cast<std::uint8_t>(soaRearCurtainStatus.curtainState);
                        if (soaRearCurtainStatus.curtainState == eevp::control::SoaRctnState::kFULLY_UP)
                        {
                            mLogger.LogInfo() << "rear curtain operation finished";
                            break;
                        }
                    }
                    if (!result) 
                    {
                        mLogger.LogInfo() << "rear curtain operation unsuccessful";
                    }
                }
                break;
            
            default:
                mLogger.LogInfo() << "Unknown detectCount value";
                break;
        }
    }
}

bool
KATECH::checkRctnReady(){
    mLogger.LogInfo() << __func__;
    getSoaRctnStatus(soaRearCurtainStatus);
    if ((soaRearCurtainStatus.errorState == eevp::control::SoaErrorState::kOK) &&\
    (soaRearCurtainStatus.curtainState != eevp::control::SoaRctnState::kMOVING_UP)&&\
    (soaRearCurtainStatus.curtainState != eevp::control::SoaRctnState::kMOVING_DOWN)&&\
    (soaRearCurtainStatus.isNormal == eevp::control::SoaDeviceIsNormal::kNORMAL))
    {
        mLogger.LogInfo() << "Rctn ready!";
        return true;
    }
    else 
    {
        mLogger.LogInfo() << "Rctn NOT ready!";
        mLogger.LogInfo() << "errorState is " << static_cast<std::uint8_t>(soaRearCurtainStatus.errorState); 
        mLogger.LogInfo() << "curtainState is " << static_cast<std::uint8_t>(soaRearCurtainStatus.curtainState); 
        mLogger.LogInfo() << "isNormal is " << static_cast<std::uint8_t>(soaRearCurtainStatus.isNormal); 

        return false;
    }
}

void
KATECH::getSoaRctnStatus(eevp::control::SoaRctnStatus& fieldValue) {
    mLogger.LogInfo() << __func__;

    rearCurtainProxyImpl->getSoaRctnStatus(fieldValue);
    if (fieldValue.errorState == eevp::control::SoaErrorState::kOK) {
        mLogger.LogInfo() << "getSoaRctnStatus is kOK";
    }
    if (fieldValue.errorState == eevp::control::SoaErrorState::kERROR) {
    mLogger.LogInfo() << "getSoaRctnStatus is kERROR";
    }
}

bool
KATECH::requestRearCurtainOperation(const eevp::control::SoaRctnMotorDir& motorDir) {
    mLogger.LogInfo() << __func__;

    eevp::control::SoaErrorState errorState = rearCurtainProxyImpl->requestRearCurtainOperation(motorDir);
    if (errorState == eevp::control::SoaErrorState::kERROR) {
        mLogger.LogInfo() << "rearcurtain opration is kERROR";
        return false;
    }
    if (errorState == eevp::control::SoaErrorState::kOK) {
        mLogger.LogInfo() << "rearcurtain operation is kOK";
        return true;
    }
}

void
KATECH::requestRearCurtainPosition(const std::uint8_t& posPercentage) {
    mLogger.LogInfo() << __func__;

    rearCurtainProxyImpl->requestRearCurtainPosition(posPercentage);
}

/// RearCurtain End


bool
KATECH::startRoaProxy() {
    mLogger.LogInfo() << __func__;
    roaProxyImpl = std::make_shared<eevp::control::roa::RoaProxyImpl>();
    auto roaListener = std::make_shared<RoaListener>(this);
    roaProxyImpl->setEventListener(roaListener);
    roaProxyImpl->init();
    return true;
}

bool
KATECH::startRearCurtainProxy() {
    mLogger.LogInfo() << __func__;
    rearCurtainProxyImpl = std::make_shared<eevp::control::rearcurtain::RearCurtainProxyImpl>();
    //auto rearCurtainListener = std::make_shared<RearCurtainListener>(this);
    //rearCurtainProxyImpl->setEventListener(rearCurtainListener);
    rearCurtainProxyImpl->init();
    return true;
}

bool
KATECH::startSubscriptionManagementProxy() {
    mLogger.LogInfo() << __func__;
    subscriptionManagementProxyImpl = std::make_shared<eevp::subscription::service::SubscriptionManagementProxyImpl>();
    auto subscriptionManagementListener = std::make_shared<SubscriptionManagementListener>(this);
    subscriptionManagementProxyImpl->setEventListener(subscriptionManagementListener);
    subscriptionManagementProxyImpl->init();
    return true;
}

} // namespace control
} // namespace eevp
