#include "ara/core/initialization.h"
#include "swc_sesl.h"

int main(void) {
    if (!ara::core::Initialize()) {
        return EXIT_FAILURE;
    }

    sesl_executable::rootswcomponent::SWC_SESL swc_SESL;
    if (swc_SESL.Start()) {
        swc_SESL.Run();
        swc_SESL.Terminate();
    }

    if (!ara::core::Deinitialize()) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;    // 0
}
