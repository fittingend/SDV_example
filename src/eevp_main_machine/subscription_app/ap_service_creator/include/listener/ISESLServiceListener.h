#ifndef __ISESLSERVICELISTENER_H__
#define __ISESLSERVICELISTENER_H__

#include "ara/SESL/impl_type_vehicle_data.h"

namespace eevp
{
    namespace simulation
    {
        class ISESLServiceListener
        {
        public:
            virtual ~ISESLServiceListener() {};

            virtual void SESL_Receive(ara::SESL::Vehicle_Data &Receive_Argument) = 0;
            virtual void SESL_Send(const ara::SESL::Vehicle_Data &Send_Argument) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __ISESLSERVICELISTENER_H__