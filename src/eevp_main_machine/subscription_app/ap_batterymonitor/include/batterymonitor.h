#ifndef batterymonitor_h_
#define batterymonitor_h_

#include <csignal>
#include <thread>
#include <ara/log/logger.h>
#include <condition_variable>
#include <mutex>

#include "eevp/control/soamlm_proxy.h"
#include "eevp/subscription/service/subscriptionmanagement_proxy.h"

#include "IMlmListener.h"
#include "ISubscriptionManagementListener.h"
#include "IBmsInfoListener.h"

#include "MoodLampProxyImpl.h"
#include "SubscriptionManagementProxyImpl.h"
#include "BmsInfoProxyImpl.h"

#include "batterymonitor_subfunction.h"

namespace eevp {
namespace control {

class BATTERYMONITOR {
public:
    /// @brief Constructor
    BATTERYMONITOR();
    /// @brief Destructor
    ~BATTERYMONITOR();

    /// @brief Start S/W Component
    bool Start();
    /// @brief Run S/W Component
    void Run();
    /// @brief Terminate S/W Component
    void Terminate();

    /// Mood Lamp
    void RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex);
    void RequestMlmSetBrightness(const std::uint16_t& brightness);

    //Subscription Management
    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& subscriptionInfo);
    void getSubscriptionInfo();

    //BMSInfo
    void ems_BmsInfo(const eevp::bmsinfo::Struct_BmsInfo& bmsInfo);


    /// @brief App name
    static const eevp::type::String mAppname;


private:
    /// @brief Signal Handler
    static void SignalHandler(std::int32_t signal);

    /// @brief Find handler
    void StartFindCallback(ara::com::ServiceHandleContainer<eevp::control::proxy::SoaMlmProxy::HandleType> services, ara::com::FindServiceHandle handle);

    /// @brief set Running State
    bool setRunningState();

    /// @brief Find Control Proxy
    bool startMlmProxy();
    /// @brief Find MoodLamp Proxy
    bool startBMSInfoProxy();
    /// @brief Find BMSInfo Proxy
    bool startSubscriptionManagementProxy();
    /// @brief Flag of Running
    static std::atomic_bool mRunning;
    /// @brief Logger
    ara::log::Logger& mLogger;
    /// @brief Subscription Flag
    bool mSubscription;
    /// BMSInfo Flag
    bool bFlag_BMSInfo_ListenerRx;

    /// lamp - start
    uint8_t brightness;
    uint8_t brightness_prev;
    /// lamp - finish

    /// socket - start //below structure are defined in subfunction.h
    Socket_Data   socket_data;
    //socket - end

    std::mutex mSubscriptionMutex;
    std::condition_variable mSubscriptionCv;

    std::shared_ptr<eevp::control::moodlamp::MoodLampProxyImpl> moodLampProxyImpl;
    std::shared_ptr<eevp::bmsinfosrv::BmsInfoProxyImpl> bmsInfoProxyImpl;
    std::shared_ptr<eevp::subscription::service::SubscriptionManagementProxyImpl> subscriptionManagementProxyImpl;

    /// @brief Instance of Port {BATTERYMONITOR.RPortSubscriptionManagement}
    //std::unique_ptr<BATTERYMONITOR::aa::port::RPortSubscriptionManagement> m_RPortSubscriptionManagement;
};

}   // namespace control
}   // namespace eevp

#endif 
