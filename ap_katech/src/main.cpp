#include "ara/core/initialization.h"
#include "katech.h"

int main(void) {
    if (!ara::core::Initialize()) {
        return EXIT_FAILURE;
    }

    eevp::control::KATECH app;
    if (app.Start()) {
        app.Run();
        app.Terminate();
    }

    if (!ara::core::Deinitialize()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    // 0
}
