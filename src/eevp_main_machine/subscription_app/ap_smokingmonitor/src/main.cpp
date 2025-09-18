#include "ara/core/initialization.h"
#include "smokingmonitor.h"

int main(void) {
    if (!ara::core::Initialize()) {
        return EXIT_FAILURE;
    }

    eevp::control::SMOKINGMONITOR app;
    if (app.Start()) {
        app.Run();
        app.Terminate();
    }

    if (!ara::core::Deinitialize()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    // 0
}
