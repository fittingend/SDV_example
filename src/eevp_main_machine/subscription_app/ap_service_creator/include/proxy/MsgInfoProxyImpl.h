#ifndef __MSGINFO_PROXY_IMPL_H__
#define __MSGINFO_PROXY_IMPL_H__

#include <condition_variable>
#include <mutex>
#include <thread>

#include "batt/msginfo/msginfosrv_proxy.h"

#include "listener/IMsgInfoListener.h"

#include "ara/log/logger.h"

namespace eevp {
namespace simulation {

class MsgInfoProxyImpl {
public:
    MsgInfoProxyImpl();
    ~MsgInfoProxyImpl();

    void setEventListener(std::shared_ptr<eevp::simulation::IMsgInfoListener> _listener);
    bool init();

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<batt::msginfo::proxy::MsgInfoSrvProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeEvent();
    /// @brief Unsubscribe Event
    void UnsubscribeEvent();

    // callback func
    void cbEms_MsgInfo();

    ara::log::Logger& mLogger;
    std::shared_ptr<eevp::simulation::IMsgInfoListener> listener;
    std::shared_ptr<batt::msginfo::proxy::MsgInfoSrvProxy> mProxy;
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;

    std::mutex mHandle;
    std::condition_variable cvHandle;
};

} // namespace simulation
} // namespace eevp

#endif /// __MSGINFO_PROXY_IMPL_H__