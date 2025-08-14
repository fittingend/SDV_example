#ifndef __IMSGINFOLISTENER_H__
#define __IMSGINFOLISTENER_H__

#include "bms/output/impl_type_outputdata.h"

namespace eevp
{
    namespace simulation
    {
        class IMsgInfoListener
        {
        public:
            virtual ~IMsgInfoListener() {};

            virtual void notifyMsgInfo(const bms::output::OutputData &output) = 0;
        };
    } /// namespace control
} /// namespace eevp

#endif // __IMSGINFOLISTENER_H__