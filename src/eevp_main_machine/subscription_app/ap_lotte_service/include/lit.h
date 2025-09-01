#pragma once

#include <csignal>
#include <thread>
#include <atomic>
#include <ara/log/logger.h>
#include <condition_variable>
#include <mutex>
#include <ara/core/optional.h>
#include <map>

#include "ISubscriptionManagementListener.h"
#include "IDmsListener.h"
#include "ISmartFilmListener.h"
#include "eevp/control/soadms_proxy.h"
#include "eevp/control/soasmartfilm_proxy.h"

// 전방 선언
namespace eevp {
namespace subscription {
namespace service {
    class SubscriptionManagementProxyImpl;
}
}
namespace control {
namespace dms {
    class DmsProxyImpl;
}
namespace smartfilm {
    class SmartFilmProxyImpl;
}
}
}

namespace eevp {
namespace control {

// 투명도 정보 관리를 위한 구조체
struct TransparencyInfo {
    bool isChanged{false};
    std::uint8_t currTransparency{0};
    std::int32_t remainingTime{0};
};

class LIT : public eevp::subscription::service::ISubscriptionManagementListener,
            public eevp::control::dms::IDmsListener,
            public eevp::control::smartfilm::ISmartFilmListener {
public:
    LIT();
    ~LIT();
    
    bool Initialize();
    void Run();
    bool Start();
    void Terminate();

    // Subscription Management
    void notifySubscriptionInfo(const eevp::subscription::type::SubscriptionInfo& subscriptionInfo) override;
    void getSubscriptionInfo();

    // DMS Listener
    void notifyDmsStatus(const eevp::control::SoaDmsStatus& status) override;
    // SmartFilm Listener
    void notifyFilmDeviceNormal(const eevp::control::SoaDeviceIsNormal& status) override;

    static const eevp::type::String mAppname;

private:
    static void SignalHandler(std::int32_t signal);

    ara::log::Logger& mLogger;
    
    bool setRunningState();
    bool startSubscriptionManagementProxy();
    bool startDmsProxy();
    bool startSmartFilmProxy();

    static std::atomic_bool mRunning;

    bool mSubscription;
    std::mutex mSubscriptionMutex;
    std::condition_variable mSubscriptionCv;

    std::shared_ptr<eevp::control::dms::DmsProxyImpl> dmsProxyImpl;
    std::shared_ptr<eevp::control::smartfilm::SmartFilmProxyImpl> smartFilmProxyImpl;
    std::shared_ptr<eevp::subscription::service::SubscriptionManagementProxyImpl> subscriptionManagementProxyImpl;

    // For Business Logic
    std::mutex m_dmsMutex;
    ara::core::Optional<eevp::control::SoaDmsStatus> m_dmsStatus;
    ara::core::Optional<eevp::control::SoaDeviceIsNormal> m_filmDeviceNormal;
    std::mutex m_smartfilmMutex;

    // 윈도우별 투명도 및 타이머 정보
    std::map<eevp::control::SoaFilmPos, TransparencyInfo> transparencyMap;
    std::uint8_t initTransparency{30}; // 초기 투명도 값
};

} // namespace control
} // namespace eevp