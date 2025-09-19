#include "smokingmonitor.h"
#include "ara/exec/execution_client.h"
#include <ctime>

#define CNT_LMT_MOODLAMP 40
#define CNT_HYSTERESIS_NONSMOKING 30

namespace eevp {
namespace control {

// eevp::control::SoaRctnStatus soaRearCurtainStatus;
//eevp::subscription::type::SubscriptionInfo subscriptionInfo;
const eevp::type::String SMOKINGMONITOR::mAppname = "ap_smokingmonitor";
std::atomic_bool SMOKINGMONITOR::mRunning(false);

/*
init()
 └─ StartFindService()
      └─ FindServiceCallback() → mProxy 생성 + SubscribeSubscriptionInfo()

         → SetReceiveHandler(cbSubscriptionInfo)
         → Subscribe(10)

             └─ notifySubscriptionInfo 이벤트 수신됨
                   └─ cbSubscriptionInfo() 호출됨
                         └─ listener->notifySubscriptionInfo(...) → SMOKINGMONITOR::notifySubscriptionInfo()
*/
// Subscription 이벤트 받는 리스너
// 구독 정보 들어오면 SMOKINGMONITOR에 전달함
class SubscriptionManagementListener 
    : public eevp::subscription::service::ISubscriptionManagementListener {
public:
    //SubscriptionManagementListener 객체 만들 때 SMOKINGMONITOR 인스턴스의 포인터를 받아서 내부 멤버 변수 smokingmonitor 으로 저장하는 것. 이후에 이 포인터를 통해 SMOKINGMONITOR의 메서드를 호출
    explicit SubscriptionManagementListener(SMOKINGMONITOR* appInstance)
        : smokingmonitor(appInstance) {}

    // 구독 정보 들어오면 SMOKINGMONITOR에 넘김
    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& info) override
    {
        // SMOKINGMONITOR 객체의 함수에 구독 정보 전달
        smokingmonitor->notifySubscriptionInfo(info);
    }

private:
    SMOKINGMONITOR* smokingmonitor;  // SMOKINGMONITOR 애플리케이션 인스턴스에 대한 포인터
};

SMOKINGMONITOR::SMOKINGMONITOR() :
        mLogger(ara::log::CreateLogger("TSMT", "SWC", ara::log::LogLevel::kInfo)),
        mSubscription(false),
        brightness(false),
        brightness_prev(false),
        // driver_status{},
        // hvac_status{},
        // filmopacity_array{},
        moodLampProxyImpl{nullptr},
        subscriptionManagementProxyImpl{nullptr}, 
        dmsProxyImpl{nullptr},
        hvacProxyImpl{nullptr},
        smartFilmProxyImpl{nullptr}
{
    mLogger.LogInfo() << __func__;
    // std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
}


SMOKINGMONITOR::~SMOKINGMONITOR() {
}

void
SMOKINGMONITOR::SignalHandler(std::int32_t /*signal*/) {
    mRunning = false;
}

bool SMOKINGMONITOR::Start() {
    mLogger.LogInfo() << __func__;

    mRunning = true;

    if (!setRunningState()) {mLogger.LogInfo() << "setrunning error"; return false;}
    if (!startSmartFilmProxy()) {mLogger.LogInfo() <<"startsmartfilm error"; return false;}
    if (!startHvacProxy()) {mLogger.LogInfo() <<"starthvac error"; return false;}
    if (!startDmsProxy()) {mLogger.LogInfo() <<"startdms error"; return false;}
    if (!startSubscriptionManagementProxy()) {mLogger.LogInfo() << "startsubscribe error"; return false;}
    if (!startMlmProxy()) {mLogger.LogInfo() << "startmlm error"; return false;}

    getSubscriptionInfo();  // 최초 구독 정보 조회하고 구독상태 상관없이 run 으로 무조건 진입
    
    return true;
}

void SMOKINGMONITOR::Run() {
    mLogger.LogInfo() << __func__;
    mLogger.LogInfo() <<"SmokingMonitor Run!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";

    static STS_SMKMON sts_smkmon = STS_SMKMON_INIT;
    static STS_SMKMON sts_smkmon_prev = STS_SMKMON_INIT;
    static uint8_t cnt4moodlamp = 0;
    static uint8_t cnt4nonsmokingstate = 0;
    eevp::control::soaDmsDriverStatus driver_status;
    static eevp::control::SoaHvacStatus hvac_status_before_smoking; 
    static eevp::control::SoaFilmOpacityArray filmopacity_array_before_smoking;
    static uint32_t gettercnt_success = 0;
    static uint32_t gettercnt_fail = 0;
    static uint8_t cnt4successrate = 0;

    while (mRunning) {
        std::unique_lock<std::mutex> lock(mSubscriptionMutex);
        
        //타임아웃 5초마다 깨어나서 상태 확인
        if (!mSubscriptionCv.wait_for(lock, std::chrono::seconds(5),
            [&]() { return mSubscription || !mRunning; })) {
            mLogger.LogInfo() << "Still unsubscribed... waiting.";
            continue;
        }

        if (!mRunning) {mLogger.LogInfo() << "App is finished..."; break;} // 종료 신호 받으면 나감

        mLogger.LogInfo() << "App is subscribed. Starting main logic.";

        while (mSubscription && mRunning) {
            mLogger.LogInfo() << "SmokingMonitor APP is running!!!!!!";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            //check every loop
            if(getSoaDmsDriverStatus(driver_status) == true)    gettercnt_success++;
            else                                                gettercnt_fail++;
            
            // 5 seconds
            cnt4successrate++;
            if(cnt4successrate == 10)
            {
                cnt4successrate = 0;
                if(gettercnt_success == 0)  {mLogger.LogInfo() << "[GETTER] getter success rate : 0%, "<< ", success cnt : "<<gettercnt_success<<", fail cnt : "<<gettercnt_fail;}
                else if(gettercnt_fail == 0){mLogger.LogInfo() << "[GETTER] getter success rate : 100%"<< ", success cnt : "<<gettercnt_success<<", fail cnt : "<<gettercnt_fail;}
                else                        {mLogger.LogInfo() << "[GETTER] getter success rate : " << (float)gettercnt_success / (gettercnt_success + gettercnt_fail) * 100.0f <<"%"
                                                                    << ", success cnt : "<<gettercnt_success<<", fail cnt : "<<gettercnt_fail;}
            }

            switch(sts_smkmon)
            {
                case STS_SMKMON_INIT :
                    mLogger.LogInfo() << "smoking monitor init";
                    if(sts_smkmon_prev != sts_smkmon)
                        sts_smkmon_prev = sts_smkmon;

                    //dms - init
                    SetDmsPower(0x1);
                    //hvac - init
                    SetHvacPower(0x1);   SetBlowingDirection(eevp::control::SoaHvacBlowDir::kFOOT);
                    SetBlowingForce(0u); SetAirSource(eevp::control::SoaHvacAirSource::kINNER);
                    //smartfilm - init
                    SetAllOpacities(100u); SetAuto(0x0);
                    //moodlamp - init
                    RequestMlmSetRgbColor(0x0); RequestMlmSetBrightness(0x0);

                    sts_smkmon = STS_SMKMON_NORMAL;
                    break;

                case STS_SMKMON_NORMAL :
                    mLogger.LogInfo() << "smoking monitor normal";
                    if(sts_smkmon_prev != sts_smkmon)
                        sts_smkmon_prev = sts_smkmon;

                    // getSoaDmsDriverStatus(driver_status);    //check every loop
                    if(driver_status.smoking == eevp::control::SoaDmsSmoking::kSMOKE)
                        sts_smkmon = STS_SMKMON_SMOKING;
                    break;

                case STS_SMKMON_SMOKING :
                    mLogger.LogInfo() << "smoking monitor smoking";
                    if(sts_smkmon_prev != sts_smkmon) 
                    {
                        if(sts_smkmon_prev == STS_SMKMON_NORMAL)            //prev state == normal -> Smoking Warn Active
                        {
                            sts_smkmon_prev = sts_smkmon;
                            //상태 원복을 위해 흡연 전 상태 저장
                            getSoaHvacSetting(hvac_status_before_smoking);
                            getSoaFilmOpacities(filmopacity_array_before_smoking);
                        
                            //hvac - smoking
                            SetHvacPower(0x1);      SetBlowingDirection(eevp::control::SoaHvacBlowDir::kFOOT_WITH_WS);
                            SetBlowingForce(100u);  SetAirSource(eevp::control::SoaHvacAirSource::kOUTER);
                            
                            //smartfilm - smoking
                            SetAllOpacities(0u);
            
                            //moodlamp - smoking
                            cnt4moodlamp = 0;

                            SendSocket();
                         }
                         else                                                //prev state != normal -> Smoking Warn Deactive
                         {
                            //moodlamp - smoking deactive
                            cnt4moodlamp = CNT_LMT_MOODLAMP+1;
                         }
                    }

                    //moodlamp - smoking
                    if(cnt4moodlamp <= CNT_LMT_MOODLAMP) 
                    {
                        cnt4moodlamp++;
                        if(cnt4moodlamp %2 == 1) { RequestMlmSetRgbColor(0x1); RequestMlmSetBrightness(0x9); }
                        else			         { RequestMlmSetRgbColor(0x1); RequestMlmSetBrightness(0x0); }
                    }
                    else 
                    {
                        cnt4moodlamp = CNT_LMT_MOODLAMP+1;
                    }

                    // getSoaDmsDriverStatus(driver_status);    //check every loop
                    if(driver_status.smoking != eevp::control::SoaDmsSmoking::kSMOKE)
                        sts_smkmon = STS_SMKMON_NONSMOKING_DETECT;
                    break;

                case STS_SMKMON_NONSMOKING_DETECT :
                    mLogger.LogInfo() << "smoking monitor nonsmoking detect";
                    if(sts_smkmon_prev != sts_smkmon) 
                    {
                        sts_smkmon_prev = sts_smkmon;
                        //moodlamp - nonsmoking
                        RequestMlmSetRgbColor(0x1); RequestMlmSetBrightness(0x0);
                        cnt4nonsmokingstate = 0;
                    }
                    // getSoaDmsDriverStatus(driver_status);    //check every loop
                    if(driver_status.smoking != eevp::control::SoaDmsSmoking::kSMOKE)
                    {   
                        cnt4nonsmokingstate++;
                        if(cnt4nonsmokingstate >= CNT_HYSTERESIS_NONSMOKING)
                            sts_smkmon = STS_SMKMON_NONSMOKING;
                    }
                    else
                        sts_smkmon = STS_SMKMON_SMOKING;
                    break;

                case STS_SMKMON_NONSMOKING :
                    mLogger.LogInfo() << "smoking monitor nonsmoking";
                    if(sts_smkmon_prev != sts_smkmon) 
                    {
                        sts_smkmon_prev = sts_smkmon;
        
                        //hvac
                        SetHvacPower(0x1);                                          SetBlowingDirection(hvac_status_before_smoking.blowingDir);
                        SetBlowingForce(hvac_status_before_smoking.blowingForce);   SetAirSource(hvac_status_before_smoking.airSrc);
                        
                        
                        
                        //smartfilm
                        for(eevp::control::SoaFilmPos i=eevp::control::SoaFilmPos::kWINDSHIELD;
                                                        i<=eevp::control::SoaFilmPos::kREAR_GLASS;
                                                        i = static_cast<eevp::control::SoaFilmPos>(static_cast<int>(i) + 1))
                        {
                            size_t index = static_cast<size_t>(i);
                            SetOpacity(i, filmopacity_array_before_smoking[index]);
                        }

                        //moodlamp
                        RequestMlmSetRgbColor(0x0); RequestMlmSetBrightness(0x0);
                    }

                    sts_smkmon = STS_SMKMON_NORMAL;
                    break;
            }
            
            // 중간에 구독이 끊기면 break
            if (!mSubscription) {
                mLogger.LogInfo() << "App is unsubscribed. Pausing logic.";
                break;
            }
        }
    }
}

void SMOKINGMONITOR::Terminate() {
    std::lock_guard<std::mutex> lock(mSubscriptionMutex);
    mRunning = false;
    mSubscription = false;
    mSubscriptionCv.notify_all();  // Run 루프 탈출
    mLogger.LogInfo() << "App is terminating.";
}

bool
SMOKINGMONITOR::setRunningState() {
    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (exec.HasValue()) {
        mLogger.LogInfo() << "SMOKINGMONITOR app in Running State";
    } else {
        mLogger.LogError() << exec.Error().Message();
        return false;
    }
    return true;
}

/// SendSocket Start

int SMOKINGMONITOR::toBCD(int num) {
    //예외처리
    if (num < 0 || num > 99) return 0;

    // 10의 자리와 1의 자리를 BCD 형식으로 변환
    int bcd = ((num / 10) << 4) | (num % 10);
    return bcd;
}

void SMOKINGMONITOR::SendSocket(void)
{
    //Header
    Socket_Header socket_header;
    socket_header.SEQ = 0; socket_header.Retry_Cnt = 0; socket_header.Length = sizeof(Socket_Data); socket_header.Res = 0;

    //Data
    Socket_Data socket_data;
    socket_data.VehicleUniqueSnr = 1;
    socket_data.AppType = 13u;
    socket_data.AppVer[0] = 0U; socket_data.AppVer[1] = 1U; socket_data.AppVer[2] = 0U;
    socket_data.AppVer[3] = 1U; socket_data.AppVer[4] = 0U; socket_data.AppVer[5] = 1U;
    time_t now = std::time(nullptr); now += 9 * 60 * 60;  tm* kstTime = std::gmtime(&now);    
    socket_data.Date[0] = (kstTime->tm_year + 1900) / 100;  socket_data.Date[1] = (kstTime->tm_year + 1900) % 100;  
    socket_data.Date[2] = kstTime->tm_mon + 1;              socket_data.Date[3] = kstTime->tm_mday;                 
    socket_data.Date[4] = kstTime->tm_hour;                 socket_data.Date[5] = kstTime->tm_min;              
    socket_data.Date[6] = kstTime->tm_sec;                  
    socket_data.SmokingStatus = 1u;

    for(int i=0;i<7;i++)
        socket_data.Date[i] = toBCD(socket_data.Date[i]);  

    //Frame
    uint8_t socket_frame[sizeof(Socket_Header)+sizeof(Socket_Data)+2];
    memset(&socket_frame, 0x0, sizeof(socket_frame));                                   //initialize
    socket_frame[0] = 0x02;                                                             //STX
    memcpy(&socket_frame[1], &socket_header, sizeof(socket_header));                    //Header
    memcpy(&socket_frame[1+sizeof(socket_header)], &socket_data, sizeof(socket_data));  //Data
    socket_frame[1+sizeof(socket_header)+sizeof(socket_data)] = 0x03;                   //ETX

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(14198);
    if (inet_pton(AF_INET, "3.34.160.201", &sockaddr.sin_addr) <= 0) {
        // araLog->LogVerbose() << "[BatteryMonitor Socket] Failed to setup socket address" << "\n";
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    for(int i=0;i<5;i++) 
    {
        // araLog->LogVerbose() << "[BatteryMonitor Socket] Connect try start." << "\n";
        if (connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == 0) 
        {
            // araLog->LogVerbose() << "[BatteryMonitor Socket] Connected successfully!" << "\n";
            int opt_val = 1;
            setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opt_val, sizeof(opt_val));
            break;
        } 
        else 
        {
            // araLog->LogVerbose() << "[BatteryMonitor Socket] Connect failed." << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    ssize_t bytesSent = write(sockfd, &socket_frame, sizeof(socket_frame));

    std::this_thread::sleep_for(std::chrono::seconds(1));
    close(sockfd);
}

/// SendSocket End

/// SmartFilm Start
void SMOKINGMONITOR::SetOpacity(const eevp::control::SoaFilmPos& pos, const std::uint8_t& opacity)
{
    mLogger.LogInfo() << __func__;

    smartFilmProxyImpl->SetOpacity(pos, opacity);
}

void SMOKINGMONITOR::SetAllOpacities(const std::uint8_t& opacity)                                  
{
    mLogger.LogInfo() << __func__ << opacity;

    smartFilmProxyImpl->SetAllOpacities(opacity);
}

void SMOKINGMONITOR::SetAuto(const bool& on)                                                       
{
    mLogger.LogInfo() << __func__ << on;

    smartFilmProxyImpl->SetAuto(on);
}

bool SMOKINGMONITOR::getSoaFilmOpacities(eevp::control::SoaFilmOpacityArray& status)      
{
    mLogger.LogInfo() << __func__;

    return smartFilmProxyImpl->getSoaFilmOpacities(status);
}
/// SmartFilm End

/// HVAC Start
void SMOKINGMONITOR::SetHvacPower(const bool& on) {
    mLogger.LogInfo() << __func__ << on;

    hvacProxyImpl->SetHvacPower(on);
}

void SMOKINGMONITOR::SetBlowingForce(const std::uint8_t& force) {
    mLogger.LogInfo() << __func__ << force;

    hvacProxyImpl->SetBlowingForce(force);
}

void SMOKINGMONITOR::SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir) {
    mLogger.LogInfo() << __func__;

    hvacProxyImpl->SetBlowingDirection(dir);
}

void SMOKINGMONITOR::SetAirSource(const eevp::control::SoaHvacAirSource& src) {
    mLogger.LogInfo() << __func__;

    hvacProxyImpl->SetAirSource(src);
}

bool SMOKINGMONITOR::getSoaHvacSetting(eevp::control::SoaHvacStatus& status) {
    mLogger.LogInfo() << __func__;

    return hvacProxyImpl->getSoaHvacSetting(status);
}

/// HVAC End

/// DMS Start
void SMOKINGMONITOR::SetDmsPower(const bool& on) {
    mLogger.LogInfo() << __func__;

    dmsProxyImpl->SetDmsPower(on);
}

bool SMOKINGMONITOR::getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status) {
    mLogger.LogInfo() << __func__;

    return dmsProxyImpl->getSoaDmsDriverStatus(status);
}
/// DMS End

/// SubscriptionManagement Start
void SMOKINGMONITOR::notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& value) {
    mLogger.LogInfo() << __func__;

    if (mAppname == value.appName) {
        {
            std::lock_guard<std::mutex> lock(mSubscriptionMutex);
            mSubscription = value.isSubscription;
        }

        if (value.isSubscription) {
            mLogger.LogInfo() << "App subscribed → waking up logic";
            //mSubscriptionCv.notify_all();
        } else {
            mLogger.LogInfo() << "App unsubscribed → logic will pause";
            // `Run()` 내 루프가 이 상태 보고 break 됨
        }


         mLogger.LogInfo() << "[Event listener]cbSubscriptionInfo : appName = " << value.appName
                             << ", isSubscription = " << (value.isSubscription ? "true" : "false");
    }
}
void 
SMOKINGMONITOR::getSubscriptionInfo()
{
    mLogger.LogInfo() << "SMOKINGMONITOR::getSubscriptionInfo";
    subscriptionManagementProxyImpl->getSubscriptionInfo(mAppname, mSubscription);
}
/// SubscriptionManagement End

/// MoodLamp Start
void
SMOKINGMONITOR::RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex) {
    mLogger.LogInfo() << __func__ << colorTableIndex;

    moodLampProxyImpl->RequestMlmSetRgbColor(colorTableIndex);
}

void
SMOKINGMONITOR::RequestMlmSetBrightness(const std::uint16_t& brightness) {
    mLogger.LogInfo() << __func__ << brightness;

    moodLampProxyImpl->RequestMlmSetBrightness(brightness);
}

/// MoodLamp End

bool
SMOKINGMONITOR::startMlmProxy() {
    mLogger.LogInfo() << __func__;
    moodLampProxyImpl = std::make_shared<eevp::control::moodlamp::MoodLampProxyImpl>();
    //auto rearCurtainListener = std::make_shared<RearCurtainListener>(this);
    //moodLampProxyImpl->setEventListener(rearCurtainListener);
    moodLampProxyImpl->init();
    return true;
}

bool
SMOKINGMONITOR::startSubscriptionManagementProxy() {
    mLogger.LogInfo() << __func__;
    subscriptionManagementProxyImpl = std::make_shared<eevp::subscription::service::SubscriptionManagementProxyImpl>();
    auto subscriptionManagementListener = std::make_shared<SubscriptionManagementListener>(this);
    subscriptionManagementProxyImpl->setEventListener(subscriptionManagementListener);
    subscriptionManagementProxyImpl->init();
    return true;
}

bool SMOKINGMONITOR::startDmsProxy() {
    mLogger.LogInfo() << __func__;
    dmsProxyImpl = std::make_shared<eevp::control::dms::DmsProxyImpl>();
    dmsProxyImpl->init();
    return true;
}

bool SMOKINGMONITOR::startHvacProxy() {
    mLogger.LogInfo() << __func__;
    hvacProxyImpl = std::make_shared<eevp::control::hvac::HvacProxyImpl>();
    hvacProxyImpl->init();
    return true;
}

bool SMOKINGMONITOR::startSmartFilmProxy() {
    mLogger.LogInfo() << __func__;
    smartFilmProxyImpl = std::make_shared<eevp::control::smartfilm::SmartFilmProxyImpl>();
    smartFilmProxyImpl->init();
    return true;
}

} // namespace control
} // namespace eevp