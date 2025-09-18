#ifndef EEVP_CONTROL_MOODLAMP_PROXY_IMPL_H_
#define EEVP_CONTROL_MOODLAMP_PROXY_IMPL_H_

#include <ara/log/logger.h>
#include "eevp/control/soamlm_proxy.h"

namespace eevp {
namespace control {
namespace moodlamp {

class MoodLampProxyImpl {
public:
    MoodLampProxyImpl();
    ~MoodLampProxyImpl();

    //void setEventListener(const std::shared_ptr<eevp::control::moodlamp::IMoodLampListener> _listener);
    bool init();

    // method
    void RequestMlmSetRgbColor(const std::uint8_t& colorTableIndex);
    void RequestMlmSetBrightness(const std::uint16_t& brightness);

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaMlmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    ara::log::Logger& mLogger;
    //std::shared_ptr<eevp::control::moodlamp::IMoodLampListener> listener;
    std::shared_ptr<eevp::control::proxy::SoaMlmProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    std::mutex mHandle;
    std::condition_variable cvHandle;
};

} // namespace moodlamp
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_MOODLAMP_PROXY_IMPL_H_