#include "ara/core/initialization.h"
#include "lit.h"

int main(void) {
    if (!ara::core::Initialize()) {
        return EXIT_FAILURE;
    }

    eevp::control::LIT app;
    if (app.Start()) {
        app.Run();
        app.Terminate();
    }

    if (!ara::core::Deinitialize()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    // 0
}
