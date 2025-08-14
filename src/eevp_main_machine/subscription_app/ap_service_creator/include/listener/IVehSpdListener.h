#ifndef __IVEHSPD_LISTENER_H__
#define __IVEHSPD_LISTENER_H__

#include "eevp/simulation/type/impl_type_vcs_vehspd.h"

namespace eevp
{
    namespace simulation
    {
        class IVehSpdListener
        {
        public:
            virtual ~IVehSpdListener() {};

            virtual void notifyVehSpd(eevp::simulation::type::VCS_VehSpd &vehSpd) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __IVEHSPD_LISTENER_H__