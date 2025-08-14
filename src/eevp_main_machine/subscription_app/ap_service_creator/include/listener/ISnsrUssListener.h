#ifndef __ISNSRUSS_LISTENER_H__
#define __ISNSRUSS_LISTENER_H__

#include "eevp/simulation/type/impl_type_usssonarinfo.h"

namespace eevp
{
    namespace simulation
    {
        class ISnsrUssListener
        {
        public:
            virtual ~ISnsrUssListener() {};

            virtual void ntfSonarInfo(eevp::simulation::type::USSSonarInfo &ussSonarInfo) = 0;
            virtual bool isDetect() = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __ISNSRUSS_LISTENER_H__