/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <services/RoutingIPCSkeleton.hpp>
#include <ipc/IPCIntf.hpp>
#include <Log.hpp>

using namespace ara::com;
using namespace ara::core;
using namespace eevp::ipchandler::type;
using namespace eevp::ipchandler::service;

namespace routingipc {

static InstanceSpecifier kRootSwComponent{"IPCHandler/RootSwComponent/IPCHandler"};

template <typename T>
String uintToHexString(T value) {
    std::stringstream ss;
    ss << std::hex << std::setw(sizeof(T) * 2) << std::setfill('0') << static_cast<int>(value);
    return String(ss.str());
}

IPCRoutingIPCSkeleton::IPCRoutingIPCSkeleton()
    : Main_IPC_RoutingIPCSkeleton(kRootSwComponent, MethodCallProcessingMode::kEventSingleThread),
    cpLock_{},
    apLock_{},
    sysCtrlReq_{0xff}
{
    memset((void *) &cpInfo_, 0, sizeof(CpInfo));
}

IPCRoutingIPCSkeleton::~IPCRoutingIPCSkeleton()
{
    StopOfferService();
}

Future<skeleton::Main_IPC_RoutingIPCSkeleton::SystemControlResponseOutput>
IPCRoutingIPCSkeleton::SystemControlResponse(const SysCtrlResp& response)
{
    Promise<skeleton::Main_IPC_RoutingIPCSkeleton::SystemControlResponseOutput> p;
    skeleton::Main_IPC_RoutingIPCSkeleton::SystemControlResponseOutput output{};
    p.set_value(output);

    {
        std::lock_guard<std::mutex> lock(apLock_);
        // TBD : SysCtrlReq  and SysCtrlResp will be updated when the scenario is written.
        apInfo_.system_ctrl_resp = static_cast<uint8_t>(response);
        ipc::IPCIntf::getInstance().SendApInfo(apInfo_);
    }
    return p.get_future();
}

Future<skeleton::Main_IPC_RoutingIPCSkeleton::GetOneTimeSystemInfoOutput>
IPCRoutingIPCSkeleton::GetOneTimeSystemInfo()
{
    OneTimeSysInfo sysInfo;
    {
        std::lock_guard<std::mutex> lock(cpLock_);
        std::uint32_t hw_version = static_cast<int>(cpInfo_.hw_version[2]);
        std::uint32_t mcu_sw_version = static_cast<int>(cpInfo_.mcu_sw_version[3]) * 10 +
            static_cast<int>(cpInfo_.mcu_sw_version[4]);
        VersionInfo version{std::to_string(hw_version), std::to_string(mcu_sw_version)};
        sysInfo = OneTimeSysInfo{version};
    }

    Promise<skeleton::Main_IPC_RoutingIPCSkeleton::GetOneTimeSystemInfoOutput> p;
    skeleton::Main_IPC_RoutingIPCSkeleton::GetOneTimeSystemInfoOutput output{sysInfo};
    p.set_value(output);

    return p.get_future();
}

Future<skeleton::Main_IPC_RoutingIPCSkeleton::SetAPSystemInfoOutput>
IPCRoutingIPCSkeleton::SetAPSystemInfo(const String& apVersion)
{
    Promise<skeleton::Main_IPC_RoutingIPCSkeleton::SetAPSystemInfoOutput> p;
    skeleton::Main_IPC_RoutingIPCSkeleton::SetAPSystemInfoOutput output{};
    p.set_value(output);

    {
        std::lock_guard<std::mutex> lock(apLock_);
        memset(&apInfo_.ap_sw_version, 0, sizeof(apInfo_.ap_sw_version));
        memcpy(&apInfo_.ap_sw_version, apVersion.c_str(), sizeof(apInfo_.ap_sw_version));
        // TBD: esw_sw_version, not used at the moment

        ipc::IPCIntf::getInstance().SendApInfo(apInfo_);
    }

    return p.get_future();
}

Future<skeleton::Main_IPC_RoutingIPCSkeleton::ASMInfoRequestOutput>
IPCRoutingIPCSkeleton::ASMInfoRequest()
{
    Promise<skeleton::Main_IPC_RoutingIPCSkeleton::ASMInfoRequestOutput> p;
    skeleton::Main_IPC_RoutingIPCSkeleton::ASMInfoRequestOutput output{};
    p.set_value(output);

    {
        std::lock_guard<std::mutex> lock(apLock_);
        ipc::IPCIntf::getInstance().SendApInfo(apInfo_);
    }

    return p.get_future();
}

void IPCRoutingIPCSkeleton::UpdateCpInfo(const CpInfo& newInfo)
{
    std::uint32_t hw_version = static_cast<int>(cpInfo_.hw_version[2]);
    std::uint32_t mcu_sw_version = static_cast<int>(newInfo.mcu_sw_version[3]) * 10 +
            static_cast<int>(newInfo.mcu_sw_version[4]);

    logger().LogDebug() << "system_ctrl_req: " << static_cast<std::uint32_t>(newInfo.system_ctrl_req);
    logger().LogDebug() << "hw_version: " << hw_version;
    logger().LogDebug() << "mcu_sw_version: " << mcu_sw_version;
    logger().LogDebug() << "acc: " << static_cast<std::uint32_t>(newInfo.acc);
    logger().LogDebug() << "ign1: " << static_cast<std::uint32_t>(newInfo.ign1);
    logger().LogDebug() << "ign3: " << static_cast<std::uint32_t>(newInfo.ign3);
    logger().LogDebug() << "acc_cnt: " << static_cast<std::uint32_t>(newInfo.acc_cnt);

    {
        const char *p = reinterpret_cast<const char*>(&newInfo);
        String v1{};
        size_t i = 0;
        size_t index = 0;
        for (; i < offsetof(struct CpInfo, ign3_cnt); ++i) {
            String value = uintToHexString<std::uint8_t>(p[i]);

            if (i != 0 && i % 16 == 0) {
                logger().LogDebug() << "[" << index << "] " << v1;
                v1.clear();
                index += 16;
            }

            v1 += " ";
            v1 += value;
        }
        if (!v1.empty()) {
            logger().LogDebug() << "[" << index + 16 << "] " << v1;
        }
    }

    std::uint8_t newReq;
    {
        std::lock_guard<std::mutex> lock(cpLock_);
        cpInfo_ = newInfo;

        // TBD : SysCtrlReq  and SysCtrlResp will be updated when the scenario is written.
        newReq = cpInfo_.system_ctrl_req;
    }

    // No need to lock for sysCtrlReq_ since
    //   UpdateCpInfo is called from a thread,
    //   and sysCtrlReq_ is used only in this function.
    // This is to call SystemControlRequest.Send when sysCtrlReq is changed.
    // TBD : Check there is an Event or Field SOME/IP option to send only if the value is differed.
    if (newReq != sysCtrlReq_) {
        sysCtrlReq_ = newReq;
        SysCtrlReq systemSysCtrlReq = static_cast<SysCtrlReq>(sysCtrlReq_);
        SystemControlRequest.Send(systemSysCtrlReq);
    }

    SendPowerState();
}

void IPCRoutingIPCSkeleton::SendPowerState()
{
    PowerState powerState = {false, false, false};
    {
        std::lock_guard<std::mutex> lock(cpLock_);

        if (cpInfo_.acc != 0x00) {
            powerState.accState = true;
        }
        if (cpInfo_.ign1 != 0x00) {
            powerState.ign1State = true;
        }
        if (cpInfo_.ign3 != 0x00) {
            powerState.ign3State = true;
        }
    }

    PeriodicSystemInfo.Send(powerState);
}

ApInfo IPCRoutingIPCSkeleton::GetApInfo()
{
    std::lock_guard<std::mutex> lock(apLock_);
    return apInfo_;
}

CpInfo IPCRoutingIPCSkeleton::GetCpInfo()
{
    std::lock_guard<std::mutex> lock(cpLock_);
    return cpInfo_;
}

#ifdef ENABLE_TEST
void IPCRoutingIPCSkeleton::SetApInfo(const ApInfo &apInfo)
{
    std::lock_guard<std::mutex> lock(apLock_);
    apInfo_ = apInfo;
}
#endif

} // routingipc