/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <cstring>

#include <Log.hpp>
#include <ipc/ipc.hpp>
#include <ipc/IPCIntf.hpp>

namespace ipc {

static std::mutex rxLock{};
static std::mutex txLock{};

static void IPC_RxCallback(void *buf, std::size_t size) {
    if (size < sizeof(CpInfo)) {
        logger().LogError() << "size from IPCLib is not enough(" << size << "/" << sizeof(CpInfo) << ")";
    } else {
        IPCIntf::getInstance().PushCpInfo(buf);
    }
}

IPCIntf::IPCIntf()
    : EventRegister(),
      cpInfos_{}
{
#ifdef ENABLE_IPC_SHM
    ipc_init();
#endif
    RegisterIPCRxCallback(IPC_RxCallback);
}

void IPCIntf::PushCpInfo(void *buf) {
    CpInfo temp;

    std::lock_guard<std::mutex> lock(rxLock);

    memcpy(&temp, buf, sizeof(CpInfo));
    cpInfos_.push_back(temp);

    SendEvent(aa_event::kIPCRx);
}

bool IPCIntf::PopCpInfo(CpInfo& cpInfo) {
    std::lock_guard<std::mutex> lock(rxLock);

    if (!cpInfos_.empty()) {
        cpInfo = cpInfos_.front();
        cpInfos_.pop_front();
        return true;
    }

    return false;
}

bool IPCIntf::IsNotified() {
    std::lock_guard<std::mutex> lock(rxLock);

    return !cpInfos_.empty();
}

void IPCIntf::SendApInfo(const ApInfo& apInfo) {
    std::lock_guard<std::mutex> lock(txLock);

#ifdef ENABLE_TEST
    apInfo_ = apInfo;
#endif

    if (IPCSendApInfo(&apInfo) != ipc_ret_ok) {
        logger().LogError() << "Faield to send APInfo";
    }
}

#ifdef ENABLE_TEST
ApInfo IPCIntf::GetApInfo() const {
    std::lock_guard<std::mutex> lock(txLock);
    return apInfo_;
}
#endif

} // ipc