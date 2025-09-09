#ifndef EEVP_IBMS_INFO_PROXY_IMPL_H_
#define EEVP_IBMS_INFO_PROXY_IMPL_H_

#include <ara/log/logger.h>
#include "eevp/bmsinfosrv/bmsinfosrv_proxy.h"
#include "IBmsInfoListener.h"
#include "batterymonitor_subfunction.h"
// #include "batterymonitor.h"

namespace eevp {
namespace bmsinfosrv {
// namespace proxy {
    
class BmsInfoProxyImpl {
public:
    BmsInfoProxyImpl();
    ~BmsInfoProxyImpl();

    void setEventListener(std::shared_ptr<eevp::bmsinfo::service::IBmsInfoListener> _listener) ;
    bool init(); 

private:
    void FindServiceCallback(
        ara::com::ServiceHandleContainer<eevp::bmsinfosrv::proxy::BmsInfoSrvProxy::HandleType> container,
        ara::com::FindServiceHandle findHandle);

    /// @brief Subscribe Event
    void SubscribeBmsInfo();

    /// @brief Unsubscribe Event
    void UnsubscribeBmsInfo();

    /// @brief callback func
    void cbBmsInfo();

    ara::log::Logger& mLogger;
    std::shared_ptr<eevp::bmsinfo::service::IBmsInfoListener> listener;
    std::shared_ptr<eevp::bmsinfosrv::proxy::BmsInfoSrvProxy> mProxy;

    std::unique_ptr<eevp::bmsinfosrv::proxy::BmsInfoSrvProxy> mRPort{nullptr};
    /// @brief FindServiceHandle
    std::shared_ptr<ara::com::FindServiceHandle> mFindHandle;
    /// @brief Current ProxyHandle
    eevp::bmsinfosrv::proxy::BmsInfoSrvProxy::HandleType mProxyHandle;
    std::mutex mHandle;
    std::condition_variable cvHandle;
};

// } // namespace proxy
} // namespace bmsinfosrv
} // namespace eevp

#endif /// EEVP_IBMS_INFO_PROXY_IMPL_H_
