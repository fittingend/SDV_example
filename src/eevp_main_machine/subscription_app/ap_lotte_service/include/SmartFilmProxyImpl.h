#pragma once

#include <ara/log/logger.h>
#include <mutex>
#include <condition_variable>
#include "eevp/control/soasmartfilm_proxy.h"
#include "ISmartFilmListener.h"

namespace eevp {
namespace control {
namespace smartfilm {

class SmartFilmProxyImpl {
public:
    SmartFilmProxyImpl();
    ~SmartFilmProxyImpl();

    void setEventListener(std::shared_ptr<ISmartFilmListener> listener);
    bool init();
    bool getSoaFilmDeviceNormal(eevp::control::SoaDeviceIsNormal& status);
    void requestSetOpacity(eevp::control::SoaFilmPos pos, std::uint8_t opacity);

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaSmartFilmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    void SubscribeSoaFilmDeviceNormal();
    void UnsubscribeField();
    void cbSoaFilmDeviceNormal();

    ara::log::Logger& mLogger;
    std::shared_ptr<ISmartFilmListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaSmartFilmProxy> mProxy;

    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    std::mutex mHandleMutex;
    std::condition_variable mCv;
};

} // namespace smartfilm
} // namespace control
} // namespace eevp
