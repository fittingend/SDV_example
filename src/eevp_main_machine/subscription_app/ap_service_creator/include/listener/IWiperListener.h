#ifndef WIPER_LISTENER_H
#define WIPER_LISTENER_H

#include "eevp/simulation/impl_type_bcm_wipinglevel.h"
#include "eevp/simulation/impl_type_bcm_returncode.h"

namespace eevp
{
    namespace simulation
    {
        class IWiperListener
        {
        public:
            virtual ~IWiperListener() {};

            virtual void stopWiping() = 0;
            virtual void startWiping() = 0;
            virtual void setWipingInterval(const std::uint16_t &wipingInterval) = 0;
            virtual void setWipingLevel(const eevp::simulation::BCM_WipingLevel &wipingLevel) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // WIPER_LISTENER_H