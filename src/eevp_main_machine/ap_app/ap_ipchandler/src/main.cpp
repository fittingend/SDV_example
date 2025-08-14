/* Copyright 2024 Hyundai Mobis Co., Ltd. All rights reserved */

#include <ara/core/initialization.h>
#include <ara/exec/execution_client.h>

#include <Log.hpp>
#include <event/EventHandler.hpp>

using namespace ara::exec;

int app_main(int argc, char *argv[]) {

    (void) argc;
    (void) argv;
    ara::core::Initialize();

    aa_event::AAEventHandler eventHandler{};

    ExecutionClient execClient{};
    ara::core::Result<void> result = execClient.ReportExecutionState(ExecutionState::kRunning);
    if (!result) {
        logger().LogError() << "Failed to report Running State";
        ara::core::Deinitialize();
        exit(EXIT_FAILURE);
    }

    eventHandler.Run();

    ara::core::Deinitialize();

    return 0;
}

int main(int argc, char *argv[]) {
#ifndef ENABLE_TEST
    return app_main(argc, argv);
#else
    extern int test_main(int argc, char *argv[]);
    return test_main(argc, argv);
#endif
}