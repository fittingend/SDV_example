#ifndef __IENVMONITOR_LISTENER_H__
#define __IENVMONITOR_LISTENER_H__

#include "eevp/simulation/type/impl_type_tms_envzonehumidityary.h"

namespace eevp
{
    namespace simulation
    {
        class IEnvMonitorListener
        {
        public:
            virtual ~IEnvMonitorListener() {};

            virtual void notifyHumidity(eevp::simulation::type::TMS_EnvZoneHumidityAry &envZoneHumidityAry) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __IENVMONITOR_LISTENER_H__