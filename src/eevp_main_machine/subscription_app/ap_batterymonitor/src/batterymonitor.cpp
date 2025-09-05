#include "batterymonitor.h"
#include "ara/exec/execution_client.h"
#include <ctime>



namespace eevp {
namespace control {

// eevp::control::SoaRctnStatus soaRearCurtainStatus;
//eevp::subscription::type::SubscriptionInfo subscriptionInfo;
const eevp::type::String BATTERYMONITOR::mAppname = "ap_batterymonitor";
std::atomic_bool BATTERYMONITOR::mRunning(false);

/*
init()
 └─ StartFindService()
      └─ FindServiceCallback() → mProxy 생성 + SubscribeSubscriptionInfo()

         → SetReceiveHandler(cbSubscriptionInfo)
         → Subscribe(10)

             └─ notifySubscriptionInfo 이벤트 수신됨
                   └─ cbSubscriptionInfo() 호출됨
                         └─ listener->notifySubscriptionInfo(...) → BATTERYMONITOR::notifySubscriptionInfo()
*/
// Subscription 이벤트 받는 리스너
// 구독 정보 들어오면 BATTERYMONITOR에 전달함
class SubscriptionManagementListener 
    : public eevp::subscription::service::ISubscriptionManagementListener {
public:
    //SubscriptionManagementListener 객체 만들 때 BATTERYMONITOR 인스턴스의 포인터를 받아서 내부 멤버 변수 batterymonitor 으로 저장하는 것. 이후에 이 포인터를 통해 BATTERYMONITOR의 메서드를 호출
    explicit SubscriptionManagementListener(BATTERYMONITOR* appInstance)
        : batterymonitor(appInstance) {}

    // 구독 정보 들어오면 BATTERYMONITOR에 넘김
    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& info) override
    {
        // BATTERYMONITOR 객체의 함수에 구독 정보 전달
        batterymonitor->notifySubscriptionInfo(info);
    }

private:
    BATTERYMONITOR* batterymonitor;  // BATTERYMONITOR 애플리케이션 인스턴스에 대한 포인터
};

BATTERYMONITOR::BATTERYMONITOR() :
        mLogger(ara::log::CreateLogger("KATC", "SWC", ara::log::LogLevel::kInfo)),
        mSubscription(false),
        moodLampProxyImpl{nullptr},
        bmsInfoProxyImpl{nullptr},
        subscriptionManagementProxyImpl{nullptr}
{
    mLogger.LogInfo() << __func__;
    // std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
}


BATTERYMONITOR::~BATTERYMONITOR() {
}

void
BATTERYMONITOR::SignalHandler(std::int32_t /*signal*/) {
    mRunning = false;
}

bool BATTERYMONITOR::Start() {
    mLogger.LogInfo() << __func__;

    mRunning = true;

    if (!setRunningState()) {mLogger.LogInfo() << "setrunning error"; return false;}
    if (!startSubscriptionManagementProxy()) {mLogger.LogInfo() << "startsubscribe error"; return false;}
    if (!startBMSInfoProxy()) {mLogger.LogInfo() << "startbmsinfo error"; return false;}
    if (!startMlmProxy()) {mLogger.LogInfo() << "startmlm error"; return false;}

    getSubscriptionInfo();  // 최초 구독 정보 조회하고 구독상태 상관없이 run 으로 무조건 진입
    
    return true;
}

void BATTERYMONITOR::Run() {
    mLogger.LogInfo() << __func__;
    mLogger.LogInfo() <<"BatteryMonitor Run!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    while (mRunning) {
        std::unique_lock<std::mutex> lock(mSubscriptionMutex);
        
        // 타임아웃 5초마다 깨어나서 상태 확인
        if (!mSubscriptionCv.wait_for(lock, std::chrono::seconds(5),
            [&]() { return mSubscription || !mRunning; })) {
            mLogger.LogInfo() << "Still unsubscribed... waiting.";
            subscription_status = mSubscription;            //subscription_status
            continue;
        }

        subscription_status = mSubscription;            //subscription_status

        if (!mRunning) {mLogger.LogInfo() << "App is finished..."; break;} // 종료 신호 받으면 나감

        mLogger.LogInfo() << "App is subscribed. Starting main logic.";

        while (mSubscription && mRunning) {
            // 실제 동작 수행 - lampctrl
            if(brightness != brightness_prev)
                RequestMlmSetBrightness(brightness);
            brightness_prev = brightness;

            mLogger.LogInfo() << "BatteryMonitor APP is running!!!!!!";
            std::this_thread::sleep_for(std::chrono::seconds(5));

            // 중간에 구독이 끊기면 break
            if (!mSubscription) {
                mLogger.LogInfo() << "App is unsubscribed. Pausing logic.";
                break;
            }
        }
    }
}

void BATTERYMONITOR::Terminate() {
    std::lock_guard<std::mutex> lock(mSubscriptionMutex);
    mRunning = false;
    mSubscription = false;
    mSubscriptionCv.notify_all();  // Run 루프 탈출
    mLogger.LogInfo() << "App is terminating.";
}

bool
BATTERYMONITOR::setRunningState() {
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (exec.HasValue()) {
        mLogger.LogInfo() << "BATTERYMONITOR app in Running State";
    } else {
        mLogger.LogError() << exec.Error().Message();
        return false;
    }
    return true;
}

/// SubscriptionManagement Start

void BATTERYMONITOR::notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& value) {
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
BATTERYMONITOR::getSubscriptionInfo()
{
    mLogger.LogInfo() << "BATTERYMONITOR::getSubscriptionInfo";
    subscriptionManagementProxyImpl->getSubscriptionInfo(mAppname, mSubscription);
}
/// SubscriptionManagement End


/// MoodLamp Start

// void
void
BATTERYMONITOR::RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex) {
    mLogger.LogInfo() << __func__;

    moodLampProxyImpl->RequestMlmSetRgbColor(colorTableIndex);
}

void
BATTERYMONITOR::RequestMlmSetBrightness(const std::uint16_t& brightness) {
    mLogger.LogInfo() << __func__;

    moodLampProxyImpl->RequestMlmSetBrightness(brightness);
}

/// MoodLamp End


bool
BATTERYMONITOR::startMlmProxy() {
    mLogger.LogInfo() << __func__;
    moodLampProxyImpl = std::make_shared<eevp::control::moodlamp::MoodLampProxyImpl>();
    //auto rearCurtainListener = std::make_shared<RearCurtainListener>(this);
    //moodLampProxyImpl->setEventListener(rearCurtainListener);
    moodLampProxyImpl->init();
    return true;
}

bool
BATTERYMONITOR::startBMSInfoProxy() {
    mLogger.LogInfo() << __func__;
    bmsInfoProxyImpl = std::make_shared<eevp::bmsinfosrv::BmsInfoProxyImpl>();
    //auto rearCurtainListener = std::make_shared<RearCurtainListener>(this);
    //bmsInfoProxyImpl->setEventListener(rearCurtainListener);
    bmsInfoProxyImpl->init();
    return true;
}

bool
BATTERYMONITOR::startSubscriptionManagementProxy() {
    mLogger.LogInfo() << __func__;
    subscriptionManagementProxyImpl = std::make_shared<eevp::subscription::service::SubscriptionManagementProxyImpl>();
    auto subscriptionManagementListener = std::make_shared<SubscriptionManagementListener>(this);
    subscriptionManagementProxyImpl->setEventListener(subscriptionManagementListener);
    subscriptionManagementProxyImpl->init();
    return true;
}

} // namespace control
} // namespace eevp