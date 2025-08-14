/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#ifndef ROUTING_IPC_SKELETON_HPP
#define ROUTING_IPC_SKELETON_HPP

#include <mutex>
#include <memory>

#include <ipc/ipc.hpp>
#include <event/Notifier.hpp>
#include <eevp/ipchandler/service/main_ipc_routingipc_skeleton.h>

namespace routingipc {

class IPCRoutingIPCSkeleton : public eevp::ipchandler::service::skeleton::Main_IPC_RoutingIPCSkeleton {
    public:
        IPCRoutingIPCSkeleton();
        ~IPCRoutingIPCSkeleton();

        ara::core::Future<eevp::ipchandler::service::skeleton::Main_IPC_RoutingIPCSkeleton::SystemControlResponseOutput>
            SystemControlResponse(const eevp::ipchandler::type::SysCtrlResp& response) override;
        ara::core::Future<eevp::ipchandler::service::skeleton::Main_IPC_RoutingIPCSkeleton::GetOneTimeSystemInfoOutput>
            GetOneTimeSystemInfo() override;
        ara::core::Future<eevp::ipchandler::service::skeleton::Main_IPC_RoutingIPCSkeleton::SetAPSystemInfoOutput>
            SetAPSystemInfo(const ara::core::String& apVersion) override;
        ara::core::Future<eevp::ipchandler::service::skeleton::Main_IPC_RoutingIPCSkeleton::ASMInfoRequestOutput>
            ASMInfoRequest() override;

        void UpdateCpInfo(const CpInfo& cpInfo);
        ApInfo GetApInfo();
        CpInfo GetCpInfo();

    protected:
        CpInfo cpInfo_;
        ApInfo apInfo_;
        std::mutex cpLock_;
        std::mutex apLock_;
        std::uint8_t sysCtrlReq_;

        void SendPowerState();

#ifdef ENABLE_TEST
    public:
        void SetApInfo(const ApInfo &apInfo);
#endif
};

} // routingipc

#endif // ROUTING_IPC_SKELETON_HPP