#pragma once

#include <ara/log/logger.h>
#include <mutex>
#include <condition_variable>
#include "eevp/control/soasmartfilm_proxy.h"
#include "ISmartFilmListener.h"
#include "eevp/control/impl_type_soafilmpos.h"
#include "eevp/control/impl_type_soafilmopacityarray.h"

namespace eevp
{
namespace control
{
namespace smartfilm
{

class SmartFilmProxyImpl {
public :
    SmartFilmProxyImpl();
    ~SmartFilmProxyImpl();

    bool init();
    void SetOpacity(const eevp::control::SoaFilmPos& pos, const std::uint8_t& opacity); // Method 추가
    void SetAllOpacities(const std::uint8_t& opacity);                                  // Method 추가
    void SetAuto(const bool& on);                                                       // Method 추가
    bool getSoaFilmOpacities(eevp::control::SoaFilmOpacityArray& status);               // Getter

private :
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaSmartFilmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    void SubscribeSoaFilmOpacities();
    void UnsubscribeField();
    void cbSoaFilmOpacities();

    ara::log::Logger& mLogger;
    std::shared_ptr<ISmartfilmListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaSmartFilmProxy> mProxy;

    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    std::mutex mHandleMutex;
    std::condition_variable mCv;
};

}
}
}