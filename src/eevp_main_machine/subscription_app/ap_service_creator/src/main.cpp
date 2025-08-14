#include <thread>
#include <csignal>
#include "ara/core/initialization.h"
#include "ServiceCreator.h"

int main(void) {
    if (!ara::core::Initialize()) {
        return EXIT_FAILURE;
    }

    eevp::simulation::ServiceCreator service_app;
    if (service_app.Start()) {
        service_app.Run();
        service_app.Terminate();
    }

    if (!ara::core::Deinitialize()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    // 0
}
