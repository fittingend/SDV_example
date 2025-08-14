#include "ara/core/initialization.h"
#include "ara/exec/execution_client.h"

#include "SubscriptionManager.h"

int
main(int ac, char** av) {
    if (!ara::core::Initialize()) {
        return EXIT_FAILURE;
    }

    ara::exec::ExecutionClient executionClient;
    auto exec = executionClient.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    if (!exec.HasValue()) {
        return EXIT_FAILURE;
    }

    eevp::subscription::SubscriptionManager eevp_app;
    if (eevp_app.Start()) {
        eevp_app.Run(ac, av);
        eevp_app.Terminate();
    }

    if (!ara::core::Deinitialize()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    // 0
}
