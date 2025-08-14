/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <Log.hpp>
#include <ipc/ipc.hpp>

#ifndef ENABLE_IPC_SHM
#define chan_id_sect2_data 1
#define ipc_send(a, b, c, d) 0
#endif

#ifdef __cplusplus
extern "C" {
#endif

IPC_RxCallback ipc_rx_callback = nullptr;

void rx_callback_sect2_ctrl(const std::uint8_t instance, int chan_id, void* buf)
{
    (void) instance;
    (void) chan_id;
    (void) buf;

    logger().LogDebug() << "Called rx_callback_sect2_ctrl, instance:" << static_cast<std::uint32_t>(instance)
                        << ", chan_id" << chan_id;
}

void rx_callback_sect2_data(const std::uint8_t instance, int chan_id, void* buf, std::size_t size)
{
    (void) instance;

    logger().LogDebug() << "Called rx_callback_sect2_data, instance:" << static_cast<std::uint32_t>(instance)
                        << ", chan_id" << chan_id;
    if (chan_id == chan_id_sect2_data)
    {
        if (ipc_rx_callback != nullptr)
        {
            ipc_rx_callback(buf, size);
        }
    } else {
        logger().LogError()
            << "called invalid param !! rx_callback_sect2_data -> chan_id:" << chan_id << " , size:" << size;
    }
}

#ifdef __cplusplus
}
#endif

void RegisterIPCRxCallback(IPC_RxCallback callback) {
    ipc_rx_callback = callback;
}

ipc_return_t IPCSendApInfo(const ApInfo *ap_info)
{
    return ipc_send(inst_sect2, chan_id_sect2_data, (char*) ap_info, sizeof(ApInfo));
}