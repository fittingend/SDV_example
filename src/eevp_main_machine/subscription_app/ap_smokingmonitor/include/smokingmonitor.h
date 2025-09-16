#ifndef smokingmonitor_h_
#define smokingmonitor_h_

#include <csignal>
#include <thread>
#include <ara/log/logger.h>
#include <condition_variable>
#include <mutex>

#include "eevp/control/soamlm_proxy.h"
#include "eevp/subscription/service/subscriptionmanagement_proxy.h"

#include "IMlmListener.h"
#include "ISubscriptionManagementListener.h"
#include "IDmsListener.h"
#include "IHvacListener.h"
#include "ISmartFilmListener.h"

#include "MoodLampProxyImpl.h"
#include "SubscriptionManagementProxyImpl.h"
#include "DmsProxyImpl.h"
#include "HvacProxyImpl.h"
#include "SmartFilmProxyImpl.h"

#include "smokingmonitor_enum.h"
#include "smokingmonitor_struct.h"
#include <ctime> //시간계산

//socket - start
#include <iostream>         
#include <sys/socket.h>     
#include <arpa/inet.h>      
#include <unistd.h>         
#include <cstring>          
#include <chrono>
#include <thread>
#define TCP_NODELAY 1
//socket - end 

namespace eevp {
namespace control {

class SMOKINGMONITOR {
public:
    /// @brief Constructor
    SMOKINGMONITOR();
    /// @brief Destructor
    ~SMOKINGMONITOR();

    /// @brief Start S/W Component
    bool Start();
    /// @brief Run S/W Component
    void Run();
    /// @brief Terminate S/W Component
    void Terminate();

    /// Mood Lamp
    void RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex);
    void RequestMlmSetBrightness(const std::uint16_t& brightness);

    /// Subscription Management
    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& subscriptionInfo);
    void getSubscriptionInfo();

    /// DMS
    void SetDmsPower(const bool& on);
    bool getSoaDmsDriverStatus(eevp::control::soaDmsDriverStatus& status);

    /// HVAC
    void SetHvacPower(const bool& on);
    void SetBlowingForce(const std::uint8_t& force);
    void SetBlowingDirection(const eevp::control::SoaHvacBlowDir& dir);
    void SetAirSource(const eevp::control::SoaHvacAirSource& src);
    bool getSoaHvacSetting(eevp::control::SoaHvacStatus& status);

    /// Smart Film
    void SetOpacity(const eevp::control::SoaFilmPos& pos, const std::uint8_t& opacity);
    void SetAllOpacities(const std::uint8_t& opacity);                                 
    void SetAuto(const bool& on);                                                      
    bool getSoaFilmOpacities(eevp::control::SoaFilmOpacityArray& status);  


    /// Send Socket
    int toBCD(int num) ;
    void SendSocket(void);

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
    bool startSubscriptionManagementProxy();
    /// @brief Find DMS Proxy
    bool startDmsProxy();
    /// @brief Find HVAC Proxy
    bool startHvacProxy();
    /// @brief Find SmartFilm Proxy
    bool startSmartFilmProxy();
    /// @brief Flag of Running
    static std::atomic_bool mRunning;
    /// @brief Logger
    ara::log::Logger& mLogger;
    /// @brief Subscription Flag
    bool mSubscription;

    /// lamp
    uint8_t brightness;
    uint8_t brightness_prev;

    // /// DMS
    // eevp::control::soaDmsDriverStatus driver_status;

    // /// HVAC
    // eevp::control::SoaHvacStatus hvac_status; 

    // /// Smart Film
    // eevp::control::SoaFilmOpacityArray filmopacity_array;

    std::mutex mSubscriptionMutex;
    std::condition_variable mSubscriptionCv;

    std::shared_ptr<eevp::control::moodlamp::MoodLampProxyImpl> moodLampProxyImpl;
    std::shared_ptr<eevp::subscription::service::SubscriptionManagementProxyImpl> subscriptionManagementProxyImpl;
    std::shared_ptr<eevp::control::dms::DmsProxyImpl> dmsProxyImpl;
    std::shared_ptr<eevp::control::hvac::HvacProxyImpl> hvacProxyImpl;
    std::shared_ptr<eevp::control::smartfilm::SmartFilmProxyImpl> smartFilmProxyImpl;

    /// @brief Instance of Port {SMOKINGMONITOR.RPortSubscriptionManagement}
    //std::unique_ptr<SMOKINGMONITOR::aa::port::RPortSubscriptionManagement> m_RPortSubscriptionManagement;
};

}   // namespace control
}   // namespace eevp

#endif 