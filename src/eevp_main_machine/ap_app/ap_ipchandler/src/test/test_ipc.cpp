/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <cstring>
#include <chrono>
#include <thread>
#include <iostream>
#include <gtest/gtest.h>

#include <ipc/ipc.hpp>
#include <ipc/IPCIntf.hpp>
#include <Log.hpp>
#include <event/EventHandler.hpp>

#ifdef __cplusplus
extern "C" {
#endif
extern void rx_callback_sect2_data(const std::uint8_t instance, int chan_id, void* buf, std::size_t size);
#ifdef __cplusplus
}
#endif

constexpr std::uint32_t kWaitTimeForThread = 100;  // wait a second in order run thread
                                                   // Increase this time in 20240402 Release
                                                   // Don't know where time is spent. might be OfferService
constexpr std::uint32_t kWaitProcessing = 5;     // wait for processing
extern void SendTestEvent(std::uint32_t event);

class IPCTest : public ::testing::Test
{
    protected:
        void SetUp() override
        {
            logger().LogInfo() << "Setup";
            th = std::thread([this]() {
                this->eventHandler.Run();
            });
        }

        void TearDown() override
        {
            logger().LogInfo() << "TearDown";
            SendTestEvent(0);
            th.join();
        }

    protected:
        std::thread th;
        aa_event::AAEventHandler eventHandler;
};

TEST_F(IPCTest, SendSystemSysCtrlReq)
{
    std::uint8_t instance = 0x00;
    std::uint32_t chan_id = 1;      // temporary value
    CpInfo cpInfo;

    memset(&cpInfo, 0, sizeof(CpInfo));
    cpInfo.system_ctrl_req = 0x03;

    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeForThread));
    rx_callback_sect2_data(instance, chan_id, (void*)&cpInfo, sizeof(CpInfo));

    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitProcessing));

    CpInfo receviedCpInfo = eventHandler.GetIPCSkeleton()->GetCpInfo();
    EXPECT_EQ(cpInfo.system_ctrl_req, receviedCpInfo.system_ctrl_req);
}

TEST_F(IPCTest, SendPowerState)
{
    std::uint8_t instance = 0x00;
    std::uint32_t chan_id = 1;      // temporary value
    CpInfo cpInfo;

    memset(&cpInfo, 0, sizeof(CpInfo));
    cpInfo.acc = 0x01;
    cpInfo.ign1 = 0x01;
    cpInfo.ign3 = 0x01;

    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitTimeForThread));
    rx_callback_sect2_data(instance, chan_id, (void*)&cpInfo, sizeof(CpInfo));

    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitProcessing));

    CpInfo receviedCpInfo = eventHandler.GetIPCSkeleton()->GetCpInfo();
    EXPECT_EQ(cpInfo.acc, receviedCpInfo.acc);
    EXPECT_EQ(cpInfo.ign1, receviedCpInfo.ign1);
    EXPECT_EQ(cpInfo.ign3, receviedCpInfo.ign3);
}

TEST_F(IPCTest, SystemCotnrolResponse)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitProcessing));

    eevp::ipchandler::SysCtrlResp resp = eevp::ipchandler::SysCtrlResp::ACC_ON;
    auto future = eventHandler.GetIPCSkeleton()->SystemControlResponse(resp);

    ApInfo apInfo = ipc::IPCIntf::getInstance().GetApInfo();
    EXPECT_EQ(apInfo.system_ctrl_resp, static_cast<std::uint8_t>(resp));
}

TEST_F(IPCTest, SetAPSystemInfo)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitProcessing));

    ara::core::String apVersion{"1"};
    auto future = eventHandler.GetIPCSkeleton()->SetAPSystemInfo(apVersion);

    ApInfo apInfo = ipc::IPCIntf::getInstance().GetApInfo();
    EXPECT_STREQ((const char *)&apInfo.ap_sw_version, apVersion.c_str());
}

TEST_F(IPCTest, ASMInfoRequest)
{
    ApInfo apInfo;
    memset(&apInfo, 0, sizeof(ApInfo));

    apInfo.system_ctrl_resp = 0x01;
    apInfo.hwire_req_dcu_rst = 0x02;
    apInfo.asm_request = 0x3;
    ara::core::String apVersion{"1"};

    std::this_thread::sleep_for(std::chrono::milliseconds(kWaitProcessing));
    eventHandler.GetIPCSkeleton()->SetApInfo(apInfo);
    auto future = eventHandler.GetIPCSkeleton()->ASMInfoRequest();

    ApInfo newInfo = ipc::IPCIntf::getInstance().GetApInfo();
    EXPECT_EQ(newInfo.system_ctrl_resp, apInfo.system_ctrl_resp);
    EXPECT_EQ(newInfo.asm_request, apInfo.asm_request);
    EXPECT_EQ(newInfo.hwire_req_dcu_rst, apInfo.hwire_req_dcu_rst);
    EXPECT_EQ(newInfo.hwire_req_esw_rst, apInfo.hwire_req_esw_rst);
}