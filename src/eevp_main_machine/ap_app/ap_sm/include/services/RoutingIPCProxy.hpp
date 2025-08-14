/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef ROUTING_IPC_PROXY_HPP
#define ROUTING_IPC_PROXY_HPP

#include <mutex>
#include <memory>
#include <condition_variable>

#include <event/Notifier.hpp>
#include <eevp/ipchandler/service/main_ipc_routingipc_proxy.h>

namespace routingipc
{

class IPCRoutingIPCProxy final : public aa_event::EventRegister
{
    public:
        IPCRoutingIPCProxy();
        ~IPCRoutingIPCProxy();

        bool Initialize(std::int32_t waitSeconds);
        void Release();

        void SystemControlResponse(const eevp::ipchandler::type::SysCtrlResp &response);
        eevp::ipchandler::type::SysCtrlReq GetSysCtrlReq();

    protected:
        std::unique_ptr<eevp::ipchandler::service::proxy::Main_IPC_RoutingIPCProxy> proxy_;
        std::mutex handleLock_;
        std::condition_variable handleCv_;
        eevp::ipchandler::type::SysCtrlReq sysCtrlReq_;

        void Subscribe();
        void Unsubscribe();
        void SystemControlRequest_ReceiveHandler(void);
        void FindServiceCallback(
            ara::com::ServiceHandleContainer<eevp::ipchandler::service::proxy::Main_IPC_RoutingIPCProxy::HandleType> curHandles,
            ara::com::FindServiceHandle handle);

};

} // namespace routingipc

#endif // ROUTING_IPC_PROXY_HPP