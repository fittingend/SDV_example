#ifndef __IGEAR_LISTENER_H__
#define __IGEAR_LISTENER_H__

#include "eevp/simulation/type/impl_type_vcs_gear.h"

namespace eevp
{
    namespace simulation
    {
        class IGearListener
        {
        public:
            virtual ~IGearListener() {};

            virtual void notifyGear(eevp::simulation::type::VCS_Gear &gear) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __IGEAR_LISTENER_H__