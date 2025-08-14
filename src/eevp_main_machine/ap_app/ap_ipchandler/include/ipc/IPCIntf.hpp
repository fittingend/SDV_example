/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <mutex>
#include <deque>
#include <event/Notifier.hpp>
#include <ipc/ipc.hpp>

#ifndef IPC_INTF_HPP
#define IPC_INTF_HPP

namespace ipc
{

class IPCIntf final : public aa_event::EventRegister
{
    public:
        static IPCIntf& getInstance() {
            static IPCIntf instance{};
            return instance;
        }

        void PushCpInfo(void *buf);
        bool PopCpInfo(CpInfo& cpInfo);
        void SendApInfo(const ApInfo& apInfo);

        bool IsNotified();

    private:
        IPCIntf();
        IPCIntf(const IPCIntf&) = delete;
        IPCIntf& operator=(const IPCIntf&) = delete;

        std::deque<CpInfo> cpInfos_;

#ifdef ENABLE_TEST
    private:
        ApInfo apInfo_;

    public:
        ApInfo GetApInfo() const;
#endif
};

} // ipc

#endif // IPC_INF_HPP