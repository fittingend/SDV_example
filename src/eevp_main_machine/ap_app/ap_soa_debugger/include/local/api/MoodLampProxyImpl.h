#ifndef SOA_CONSUMER_MOODLAMP_PROXY_IMPL_H_
#define SOA_CONSUMER_MOODLAMP_PROXY_IMPL_H_

#include <thread>

#include "eevp/control/soamlm_proxy.h"
#include "IMoodLampListener.h"

namespace eevp {
namespace control {
namespace moodlamp {

class MoodLampProxyImpl {
public:
    static MoodLampProxyImpl *GetInstance();
    static void DeleteInstance();

    MoodLampProxyImpl();
    ~MoodLampProxyImpl();

    void setEventListener(const std::shared_ptr<eevp::control::moodlamp::IMoodLampListener> _listener);
    bool init();

    // method
    void requestMlmSetRGBColor(const std::uint8_t& colorTableIndex);
    void requestMlmSetMode(const eevp::control::SoaMlmMoodMode& mood);
    void requestMlmSetBrightness(const std::int16_t& brightness);

    // field getter
    bool getterSoaMlmStatus(eevp::control::SoaMlmStatus& soaMlmStatus);

    std::string GetSubscriptStatusString();

private:
    static MoodLampProxyImpl *s_mInst;

    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::control::proxy::SoaMlmProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Subscribe Field
    void SubscribeField();

    // callback func
    void cbSoaMlmStatus();

    std::shared_ptr<eevp::control::moodlamp::IMoodLampListener> mListener;
    std::shared_ptr<eevp::control::proxy::SoaMlmProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
};

} // namespace moodlamp
} // namespace control
} // namespace eevp

#endif /// EEVP_CONTROL_MOODLAMP_PROXY_IMPL_H_