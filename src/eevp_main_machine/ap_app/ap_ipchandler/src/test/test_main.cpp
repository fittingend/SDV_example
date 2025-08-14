/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <csignal>
#include <thread>
#include <chrono>

#include <gtest/gtest.h>
#include <ara/core/initialization.h>
#include <ara/exec/execution_client.h>

int test_main(int argc, char *argv[]) {
    ara::core::Initialize();
    ara::exec::ExecutionClient execClient{};
    (void) execClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    ::testing::InitGoogleTest(&argc, argv);
    int ret = RUN_ALL_TESTS();
    ara::core::Deinitialize();
    return ret;
}