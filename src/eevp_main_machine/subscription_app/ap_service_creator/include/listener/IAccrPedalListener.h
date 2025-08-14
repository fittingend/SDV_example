#ifndef __IACCRPEDAL_LISTENER_H__
#define __IACCRPEDAL_LISTENER_H__

#include "eevp/simulation/type/impl_type_vcs_accrpedal.h"

namespace eevp
{
    namespace simulation
    {
        class IAccrPedalListener
        {
        public:
            virtual ~IAccrPedalListener() {};

            virtual void notifyAccrPedal(eevp::simulation::type::VCS_AccrPedal &accrPedal) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __IACCRPEDAL_LISTENER_H__