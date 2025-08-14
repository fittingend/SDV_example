/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <Log.hpp>
#include <services/RoutingIPCProxy.hpp>

using namespace eevp::ipchandler::type;
using namespace eevp::ipchandler::service;

constexpr std::int32_t kMethodWaitCount(10);
constexpr std::int32_t kMethodWaitTime(10);
constexpr std::size_t kEventCacheSize{10};
const ara::core::InstanceSpecifier kRootSwComponent{"SM/PARA/IPCHandler"};

namespace routingipc
{

IPCRoutingIPCProxy::IPCRoutingIPCProxy()
    : proxy_{nullptr},
    handleLock_{},
    handleCv_{},
    sysCtrlReq_{SysCtrlReq::NORMAL}
{
}

IPCRoutingIPCProxy::~IPCRoutingIPCProxy()
{
}

bool IPCRoutingIPCProxy::Initialize(std::int32_t waitSeconds)
{
    std::unique_lock<std::mutex> lock(handleLock_);
    try {
        proxy::Main_IPC_RoutingIPCProxy::StartFindService(
            std::bind(&IPCRoutingIPCProxy::FindServiceCallback, this, std::placeholders::_1, std::placeholders::_2),
            kRootSwComponent);
    } catch (std::exception &e) {
        logger().LogError() << "Failed to StartFindServce(RoutingIPC) with " << e.what();
        return false;
    } catch (...) {
        logger().LogError() << "Failed to StartFindServce(RoutingIPC) with unknown reason";
        return false;
    }

    if (handleCv_.wait_for(lock, std::chrono::milliseconds(waitSeconds * 1000)) == std::cv_status::timeout) {
        logger().LogError() << "Failed to find service(RoutingIPC) over " << waitSeconds << " seconds";
        return false;
    }

    logger().LogInfo() << "Success to find RoutingIPC";
    return true;
}

void IPCRoutingIPCProxy::Release()
{
    std::lock_guard<std::mutex> lock(handleLock_);
    Unsubscribe();
    proxy_ = nullptr;
}

SysCtrlReq IPCRoutingIPCProxy::GetSysCtrlReq()
{
    std::lock_guard<std::mutex> lock(handleLock_);
    return sysCtrlReq_;
}

// No scenario yet, so not sure parameters
void IPCRoutingIPCProxy::SystemControlResponse(const SysCtrlResp &response)
{
    std::lock_guard<std::mutex> lock(handleLock_);
    if (proxy_ != nullptr) {
        try {
            // just send BOOTING now
            auto count = kMethodWaitCount;
            auto future = proxy_->SystemControlResponse(SysCtrlResp::BOOTING);

            while (!future.is_ready() && --count) {
                future.wait_for(std::chrono::milliseconds(kMethodWaitTime));
            }

            if (count <= 0) {
                logger().LogError() << "Failed to call SystemControlResponse";
            }
        } catch (std::exception &e) {
            logger().LogError() << "Failed to call SystemControlResponse with " << e.what();
        } catch (...) {
            logger().LogError() << "Failed to call SystemControlResponse with unknown reason";
        }
    }
}

void IPCRoutingIPCProxy::SystemControlRequest_ReceiveHandler(void)
{
    std::lock_guard<std::mutex> lock(handleLock_);
    if (proxy_ != nullptr) {
        proxy_->SystemControlRequest.GetNewSamples([&] (auto message) {
            sysCtrlReq_ = static_cast<SysCtrlReq>(*message);
        }, 1);

        SendEvent(aa_event::Event::kIPCSystemReq);
    }
}

void IPCRoutingIPCProxy::Subscribe()
{
    if (proxy_ != nullptr) {
        auto result = proxy_->SystemControlRequest.SetReceiveHandler(std::bind(&IPCRoutingIPCProxy::SystemControlRequest_ReceiveHandler, this));
        if (!result.HasValue()) {
            logger().LogWarn() << "Failed to SetReceiveHandler for SystemControlRequest with " << result.Error().Message();
        }

        result = proxy_->SystemControlRequest.Subscribe(kEventCacheSize);
        if (!result.HasValue()) {
            logger().LogWarn() << "Failed to Subscribe for SystemControlRequest with " << result.Error().Message();
        }
    }
}

void IPCRoutingIPCProxy::Unsubscribe()
{
    if (proxy_ != nullptr) {
        proxy_->SystemControlRequest.Unsubscribe();
    }
}

void IPCRoutingIPCProxy::FindServiceCallback(
    ara::com::ServiceHandleContainer<proxy::Main_IPC_RoutingIPCProxy::HandleType> curHandles,
    ara::com::FindServiceHandle handle)
{
    (void) handle;

    // Not consider the complex service disconnection at the moment
    // That will be implemented as necessary since not sure the possibilities
    // This is a simple way to find service.

    std::lock_guard<std::mutex> lock(handleLock_);

    if (proxy_ != nullptr) {
        Unsubscribe();
        proxy_ = nullptr;
    }

    if (!curHandles.empty()) {
        proxy_ = std::make_unique<proxy::Main_IPC_RoutingIPCProxy>(curHandles[0]);
        Subscribe();
        handleCv_.notify_one();
    }
}

} // namespace routingipc